#pragma once
#include <utility>
#include <memory>

namespace sw
{
    using Point = std::pair<uint32_t, uint32_t>;

    namespace units
    {
        class UnitInterface;
    }
    using UnitPtr = std::shared_ptr<units::UnitInterface>;
        
}