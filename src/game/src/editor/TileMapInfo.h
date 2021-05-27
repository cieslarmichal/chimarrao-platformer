#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Vector.h"
#include "Tile.h"

namespace game
{
struct TileMapInfo
{
    std::string name;
    utils::Vector2i mapSize;
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
};
}