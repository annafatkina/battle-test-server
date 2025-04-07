#pragma once
#include "Map.hpp"
//#include "UnitBuilder.hpp"
#include "Units/UnitInterface.hpp"
#include "IO/System/EventLog.hpp"
#include "Types.hpp"

#include <memory>
#include <unordered_map>
#include <functional>
#include <fstream>
#include <variant>


namespace sw
{
    class DataManager
    {
        using callback_t = std::function<void()>;

        std::shared_ptr<EventLog> _eventLog;
        std::shared_ptr<Map> _map;
        std::unordered_map<uint32_t, UnitPtr> _units;
        uint64_t _tick = 1;


    public:
        DataManager(std::shared_ptr<EventLog> log) 
        : _eventLog(std::move(log))
        {
        }

        void add_map(std::shared_ptr<Map> map)
        {
            _map = map;
        }

        void add_unit(UnitPtr unit)
        {
            _units.emplace(unit->getId(), unit);
        }

        void add_tick()
        {
            _tick++;
        }

        int getTick() const
        {
            return _tick;
        }

        std::unordered_map<uint32_t, UnitPtr>& getUnits()
        {
            return _units;
        }

        const UnitPtr getUnit(uint32_t id) const
        try {
            return _units.at(id);
        } catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            return nullptr;
        }

        const std::shared_ptr<Map> getMap() const
        {
            return _map;
        }

        bool move_unit(int from_x, int from_y, int to_x, int to_y)
        {
            auto unitId = _map->get(from_x, from_y);
            if(!unitId)
            {
                return false;
            }
            if(!_map->move_unit(from_x, from_y, to_x, to_y)) 
            {
                return false;
            }

            _units.at(unitId.value())->atPosition(to_x, to_y);
            return true;
        }
    
        void delete_unit(uint32_t id)
        {
            if(!_units.contains(id)) return;
            auto [x,y] = _units.at(id)->getPosition();
            _map->clear(x, y);
            _units.erase(id);
        }
    };
}