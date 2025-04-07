#pragma once
#include "IO/Events/Events.hpp"

#include <vector>
#include <optional>

namespace sw
{
    class Map
    {
        int _height;
        int _width;

        std::vector<std::optional<uint32_t>> _grid;

    public:
        Map(int w, int h)
            : _height(h)
            , _width(w)
            , _grid(_width * _height, std::nullopt)
        {
        }

        std::optional<uint32_t> get(int x, int y) const
        {
            return _grid[_width * x + y];
        }

        uint32_t getX() const
        {
            return _width;
        }

        uint32_t getY() const
        {
            return _height;
        }

        bool add_unit(uint32_t id, int x, int y) 
        {
            if(x >= _width || y >= _height)
            {
                throw "Failed to add unit, wrong position";
            }

            if(_grid[_width * x + y] != std::nullopt)
            {
                throw "Failed to add unit, position is already taken";
            }

            _grid[_width * x + y] = id;
            return true;
        }

        bool move_unit(int from_x, int from_y, int to_x, int to_y) 
        {
            if( from_x >= _width || to_x >= _width ||
                from_y >= _height || to_y >= _height)
            {
                return false;
            }

            _grid[_width * to_x + to_y] = _grid[_width * from_x + from_y];
            _grid[_width * from_x + from_y] = std::nullopt;
            return true;
        }

        void clear(uint32_t x, uint32_t y)
        {
            _grid[_width * x + y] = std::nullopt;
        }
    };
}