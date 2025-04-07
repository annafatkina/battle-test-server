#pragma once

#include "Strategy.hpp"
#include "Types.hpp"
#include "Units/UnitInterface.hpp"
#include "IO/Events/Events.hpp"
#include "../DataManager.hpp"


namespace sw::strategies
{
    class Combat : virtual public Strategy
    {
        sw::DataManager& _dataManager;
        uint32_t _strength;
        uint32_t _unitId;

        std::optional<uint32_t> findClosest() const
        {
            auto unit = _dataManager.getUnit(_unitId);
            auto map = _dataManager.getMap();

            auto [unitX, unitY] = unit->getPosition();
            return Strategy::findOnMap(map, unitX, unitY, 1);
        }

    public:
        Combat(sw::DataManager& dataManager, uint32_t unitId, uint32_t strength)
        : _dataManager(dataManager)
        , _unitId(unitId)
        , _strength(strength)
        {}

        virtual ~Combat() = default;

        io::Event_t act() override
        {
            if(auto closestUnit = findClosest(); closestUnit)
            {
                auto hp = _dataManager.getUnit(closestUnit.value())->getHP();
                auto new_hp = hp < _strength ? 0u : hp - _strength;

                if(new_hp == 0u)
                {
                    return io::UnitDied{closestUnit.value()};
                }

                return io::UnitAttacked{
                    .attackerUnitId = _unitId,
                    .targetUnitId = closestUnit.value(),
                    .damage = _strength,
                    .targetHp = new_hp
                };
            }
            return std::monostate{};
        }
    };
}