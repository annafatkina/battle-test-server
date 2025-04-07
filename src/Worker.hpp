#pragma once
#include "Units/UnitInterface.hpp"
#include "IO/System/EventLog.hpp"
#include "Map.hpp"
#include "Observer.hpp"
#include "DataManager.hpp"
#include "UnitBuilder.hpp"
#include "Types.hpp"

#include <unordered_map>
#include <functional>
#include <fstream>
#include <ranges>


namespace sw
{
    class Worker
    {
    private:
        std::shared_ptr<EventLog> _eventLog;
        DataManager _dataManager;
        Observer _observer;
        
        void createMapHandler(io::CreateMap command)
        {
            _dataManager.add_map(std::make_shared<Map>(command.width, command.height));
            _eventLog->log(_dataManager.getTick(), 
                io::MapCreated{.width = command.width, .height = command.height});
        }

        void marchHandler(io::March command)
        {
            _dataManager.getUnit(command.unitId)->setMarch(command.targetX, command.targetY);
            _eventLog->log(_dataManager.getTick(), 
                io::MarchStarted{.targetX = command.targetX, .targetY = command.targetY});

        }

        void spawnHunterHandler(io::SpawnHunter command)
        {
            UnitBuilder unitBuilder;
            _dataManager.add_unit(
                unitBuilder
                    .createHunter(_dataManager, command.unitId, command.strength, command.agility, 
                        command.range, command.hp)
                    .atPosition(command.x, command.y)
                    .build()
            );
            _dataManager.getMap()->add_unit(command.unitId, command.x, command.y);

            _eventLog->log(_dataManager.getTick(),
                io::UnitSpawned{
                    .unitId = command.unitId,
                    .unitType = "Hunter",
                    .x = command.x,
                    .y = command.y
                });
        }

        void spawnSwordmanHandler(io::SpawnSwordsman command)
        {
            UnitBuilder unitBuilder;
            _dataManager.add_unit(
                unitBuilder
                    .createSwordsman(_dataManager, command.unitId, command.strength, command.hp)
                    .atPosition(command.x, command.y)
                    .build()
            );
            _dataManager.getMap()->add_unit(command.unitId, command.x, command.y);

            _eventLog->log(_dataManager.getTick(),
                io::UnitSpawned{
                    .unitId = command.unitId,
                    .unitType = "Swordsman",
                    .x = command.x,
                    .y = command.y
                });
        }

        void init(const std::string& command_file)
        {
            using namespace std::placeholders;

            io::CommandParser commandParser;
            commandParser
                .add<io::CreateMap>(std::bind(&Worker::createMapHandler, this, _1))
                .add<io::March>(std::bind(&Worker::marchHandler, this, _1))
                .add<io::SpawnHunter>(std::bind(&Worker::spawnHunterHandler, this, _1))
                .add<io::SpawnSwordsman>(std::bind(&Worker::spawnSwordmanHandler, this, _1));
            
            std::ifstream file(command_file);
            commandParser.parse(file);

            for(auto&& [id, unit] : _dataManager.getUnits())
            {
                _observer.subscribe([unit](const io::Event_t& e) {
                    unit->notify(e);
                });
            }
        }
    public:
        Worker(const std::string& command_file) 
        : _eventLog(std::make_shared<EventLog>())
        , _dataManager(_eventLog)
        {
            init(command_file);
        }

        void run()
        try {
            bool is_there_action = true;
            while (is_there_action)
            {
                std::vector<io::Event_t> events;
                for(auto&& [id, unit] : _dataManager.getUnits())
                {
                    auto event = unit->act();
                    if(event.index() > 0)
                    {
                        events.push_back(event);
                    }

                    std::visit([this](auto&& value) {
                        _eventLog->log(_dataManager.getTick(), value);
                    }, event);

                    _observer.notify_all(event);
                }

                is_there_action = !events.empty();
                for(auto&& e : events)
                {
                    std::visit([this](auto&& value) {
                        using T = std::decay_t<decltype(value)>;
                        if constexpr (std::is_same_v<T, io::MarchEnded> 
                            || std::is_same_v<T, io::UnitDied>)
                        {
                            _dataManager.delete_unit(value.unitId);
                        }
                    }, e);
                }
                _dataManager.add_tick();
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception: " << e.what() << std::endl;
        }

        ~Worker() = default;
    };
}