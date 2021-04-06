#pragma once

#include <string>
#include <vector>

#include "TileMapInfo.h"

namespace game
{
class TileMapSerializer
{
public:
    virtual ~TileMapSerializer() = default;

    virtual std::string serialize(const TileMapInfo&) = 0;
    virtual TileMapInfo deserialize(const std::string&) const = 0;
};
}