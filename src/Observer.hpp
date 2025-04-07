#pragma once
#include "Units/UnitInterface.hpp"
#include "IO/Events/Events.hpp"

#include <vector>
#include <memory>
#include <functional>


namespace sw
{
    class Observer
    {
        using callback_t = std::function<void(const io::Event_t&)>;
        std::vector<callback_t> _listeners;

    public:
        Observer() = default;

        void subscribe(callback_t listener)
        {
            _listeners.emplace_back(std::move(listener));
        }

        void notify_all(const io::Event_t& e) const
        {
            for(auto&& listener: _listeners)
            {
                listener(e);
            }
        }
    };
}