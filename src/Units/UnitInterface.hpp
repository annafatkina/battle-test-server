#pragma once

#include "IO/Events/Events.hpp"
#include "Strategies/Strategy.hpp"

#include <utility>
#include <string>

namespace sw::units
{
    enum class Feature;

    class UnitInterface
    {
    protected:
        uint32_t _pos_x = 0;
        uint32_t _pos_y = 0;
        uint32_t _march_x = 0;
        uint32_t _march_y = 0;
        uint32_t _hp = 0;
        uint32_t _id = 0;

        std::shared_ptr<strategies::Strategy> _strategy;

    public:
        UnitInterface(uint32_t id, uint32_t hp, std::shared_ptr<strategies::Strategy> strategy)
            : _id(id)
            , _hp(hp)
            , _strategy(strategy)
        {}

        virtual ~UnitInterface() {}
        
        uint32_t getId() const
        {
            return _id;
        }

        uint32_t getHP() const
        {
            return _hp;
        }

        virtual void atPosition(uint32_t x, uint32_t y) final
        {
            _pos_x = x;
            _pos_y = y;
        }

        virtual std::pair<int, int> getPosition() final
        {
            return std::make_pair(_pos_x, _pos_y); 
        }
        
        virtual void setMarch(int32_t x, int32_t y)
        {
            _march_x = x;
            _march_y = y;
        }

        virtual io::Event_t act() 
        {
            if(_strategy)
            {
                return _strategy->act();
            }
           return std::monostate();
        }

        virtual void notify(const io::Event_t& e) = 0;
    };


}