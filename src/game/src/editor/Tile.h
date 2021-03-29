#pragma once

#include "TileType.h"
#include <optional>

namespace game
{
struct Tile
{
    std::optional<TileType> type;
};
}