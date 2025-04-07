#pragma once

#include "MapCreated.hpp"
#include "MarchEnded.hpp"
#include "MarchStarted.hpp"
#include "UnitAttacked.hpp"
#include "UnitDied.hpp"
#include "UnitMoved.hpp"
#include "UnitSpawned.hpp"

#include <variant>

namespace sw::io
{
    using Event_t = std::variant
        < std::monostate
        , MapCreated
        , MarchEnded
        , MarchStarted
        , UnitAttacked
        , UnitDied
        , UnitMoved
        , UnitSpawned
        >;
}