#pragma once

#include "Tile.h"
#include "TileMapSerializer.h"

namespace game{
class TileMapSerializerJson : public TileMapSerializer
{
public:
    TileMapSerializerJson();
    std::string serialize(const TileMapInfo& tileMapInfo) override;
    TileMapInfo deserialize(const std::string& string) const override;

private:
    std::unordered_map<std::optional<TileType>,int> tileTypeToInt
        {
            {std::nullopt,0}
        };
};
}