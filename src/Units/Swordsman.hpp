#pragma once

#include "UnitInterface.hpp"
#include "Strategies/Combat.hpp"
#include "Strategies/Move.hpp"

#include <unordered_map>
#include <memory>
#include <variant>

namespace sw::units
{
    class SwordsmanStrategy : public strategies::Combat, public strategies::Move
    {
    public:
        SwordsmanStrategy(sw::DataManager& dataManager,uint32_t unitId, uint32_t strength)
        : strategies::Combat(dataManager, unitId, strength)
        , strategies::Move(dataManager, unitId)
        {}

        io::Event_t act() override final
        {
            if(auto e = strategies::Combat::act(); e.index() > 0 )
            {
                return e;
            }

            return strategies::Move::act();
        }
    };

    class Swordsman : public UnitInterface
    {
        int _strength;
        
    public:
        Swordsman(sw::DataManager& dm, uint32_t id, uint32_t strength,  uint32_t hp) 
            : UnitInterface(id, hp, std::make_shared<SwordsmanStrategy>(dm, id, strength))
        {}

        ~Swordsman() override = default;

        virtual void setMarch(int32_t x, int32_t y)
        {
            UnitInterface::setMarch(x, y);
            std::dynamic_pointer_cast<SwordsmanStrategy>(_strategy)->setMarch(x, y);
        }

        io::Event_t act() override
        {
            if(_hp == 0)
            {
                return io::UnitDied(_id);
            }
            return UnitInterface::act();
        }

        void notify(const io::Event_t& e) override
        {
            std::visit([&](auto&& event) {
                using T = std::decay_t<decltype(event)>;
                if constexpr (std::is_same_v<T, io::UnitAttacked>) {
                    if(event.targetUnitId == _id)
                    {
                        _hp = event.targetHp;
                    }
                }
            }, e);
        }
    };
}