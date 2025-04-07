#pragma once

#include "Units/UnitInterface.hpp"
#include "Units/Hunter.hpp"
#include "Units/Swordsman.hpp"

#include <memory>

namespace sw
{
    class UnitBuilder
    {
    private:
        std::shared_ptr<units::UnitInterface> _unit;

    public:
        UnitBuilder() = default;
        ~UnitBuilder() = default;

        UnitBuilder& createHunter(sw::DataManager& dm, uint32_t id, uint32_t strength, uint32_t agility, 
            uint32_t range, uint32_t hp) 
        {
            _unit = std::make_shared<units::Hunter>(dm, id, strength, agility, range, hp);
            return *this;
        }


        UnitBuilder& createSwordsman(sw::DataManager& dm, uint32_t id, uint32_t strength, uint32_t hp) 
        {
            _unit = std::make_shared<units::Swordsman>(dm, id, strength,  hp);
            return *this;
        }

        UnitBuilder& atPosition(uint32_t x, uint32_t y)
        {
            _unit->atPosition(x, y);
            return *this;
        }

        std::shared_ptr<units::UnitInterface> build()
        {
            return _unit;
        }
    };
    
    
}