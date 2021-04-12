#pragma once

#include <optional>

#include "TileType.h"

namespace game
{
struct Tile
{
    std::optional<TileType> type;
};
}