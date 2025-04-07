#pragma once

#include "Strategy.hpp"
#include "Types.hpp"
#include "Units/UnitInterface.hpp"
#include "IO/Events/Events.hpp"
#include "../DataManager.hpp"


namespace sw::strategies
{
    class Melee : virtual public Strategy
    {
        sw::DataManager& _dataManager;
        uint32_t _range;
        uint32_t _agility;
        uint32_t _unitId;

        std::optional<uint32_t> findOnMap(int range) const
        {
            auto unit = _dataManager.getUnit(_unitId);
            auto map = _dataManager.getMap();

            auto [unitX, unitY] = unit->getPosition();
            return Strategy::findOnMap(map, unitX, unitY, range);
        }

        bool is_clear_nearest() const
        {
            return findOnMap(1) == std::nullopt;
        }

    public:
        Melee(sw::DataManager& dataManager, uint32_t range, uint32_t agility, uint32_t unitId)
        : _dataManager(dataManager) 
        , _range(range)
        , _agility(agility)
        , _unitId(unitId)
        {}

        virtual ~Melee() = default;

        io::Event_t act() override
        {
            if(!is_clear_nearest())
            {
                return std::monostate{};
            }

            auto toBeAttacked = findOnMap(_range);
            if(toBeAttacked == std::nullopt)
            {
                return std::monostate{};
            }

            auto hp = _dataManager.getUnit(toBeAttacked.value())->getHP();
            auto new_hp = hp < _agility ? 0u : hp - _agility;

            if(new_hp == 0u)
            {
                return io::UnitDied{
                    toBeAttacked.value()
                };
            }
            
            return io::UnitAttacked{
                .attackerUnitId = _unitId,
                .targetUnitId = toBeAttacked.value(),
                .damage = _agility,
                .targetHp = new_hp
            };
        }
    };
}
