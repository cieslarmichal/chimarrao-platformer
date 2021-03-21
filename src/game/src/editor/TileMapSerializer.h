#pragma once

#include <string>
#include <vector>

#include "Tile.h"

namespace game
{
class TileMapSerializer
{
public:
    virtual ~TileMapSerializer() = default;

    virtual std::string serialize(const std::vector<std::vector<Tile>>&) = 0;
    virtual std::vector<std::vector<Tile>> deserialize(const std::string&) const = 0;
};
}