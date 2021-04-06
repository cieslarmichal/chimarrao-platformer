#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Vector.h"

namespace game{
struct TileMapInfo{
    std::string name;
    utils::Vector2i mapSize;
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
};
}