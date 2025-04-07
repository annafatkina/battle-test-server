#pragma once

#include "Strategy.hpp"
#include "Types.hpp"
#include "Units/UnitInterface.hpp"
#include "IO/Events/Events.hpp"
#include "../DataManager.hpp"


namespace sw::strategies
{
    class Move : virtual public Strategy
    {
        sw::DataManager& _dataManager;
        uint32_t _unitId;
        uint32_t _marchX = 0;
        uint32_t _marchY = 0;

    public:
        Move(sw::DataManager& dataManager, uint32_t unitId)
        : _dataManager(dataManager)
        , _unitId(unitId)
        {}

        ~Move() = default;

        void setMarch(uint32_t x, uint32_t y)
        {
            _marchX = x;
            _marchY = y;
        }

        io::Event_t act() override
        {
            auto unit = _dataManager.getUnit(_unitId);
            auto [unitX, unitY] = unit->getPosition();

            if(_marchX == unitX && _marchY == unitY)
            {
                return io::MarchEnded(_unitId, _marchX, _marchY);
            }

            uint32_t toX = unitX, toY = unitY;
            if(_marchX > unitX)
            {
                toX++;
            } else if(_marchX < unitX)
            {
                toX--;
            }

            if(_marchY > unitY)
            {
                toY++;
            } else if (_marchY < unitY)
            {
                toY--;
            }

            auto map = _dataManager.getMap();
            if(map->get(toX, toY))
            {
                return std::monostate{};
            }

             _dataManager.move_unit(unitX, unitY, toX, toY);

            return io::UnitMoved(_unitId, toX, toY);
        }

    };
}