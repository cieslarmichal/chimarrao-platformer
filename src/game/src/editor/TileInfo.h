#pragma once

#include <optional>

#include "TileType.h"

namespace game
{
struct TileInfo
{
    std::optional<TileType> type;
};
}