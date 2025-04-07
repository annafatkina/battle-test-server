#pragma once

#include "IO/Events/Events.hpp"
#include "Map.hpp"

#include <memory>
#include <optional>


namespace sw::strategies
{
    class Strategy
    {
    protected:
        std::optional<uint32_t> findOnMap(const std::shared_ptr<Map> map, uint32_t unitX, uint32_t unitY,
             int range) const
        {
            auto xFrom = unitX < range ? 0u : unitX-range;
            auto yFrom = unitY < range ? 0u : unitY-range;
            auto xTo = std::min(map->getX()-1, unitX+range);
            auto yTo = std::min(map->getY()-1, unitY+range);

            for (auto x = xFrom; x <= xTo; x++)
            {
                for(auto y = yFrom; y <= yTo; y++)
                {
                    if(x == unitX && y == unitY)
                    {
                        continue;
                    }

                    if (auto id = map->get(x, y); id)
                    {
                        return id;
                    }
                }
            }
            return std::nullopt;
        }
    public:
        Strategy() = default;
        virtual ~Strategy() = default;

        virtual io::Event_t act() = 0;
    };
}