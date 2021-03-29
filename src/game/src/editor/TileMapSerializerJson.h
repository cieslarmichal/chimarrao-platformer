#pragma once

#include "TileMapSerializer.h"

namespace game{
class TileMapSerializerJson : public TileMapSerializer
{
public:
    TileMapSerializerJson();
    std::string serialize(const std::vector<std::vector<Tile>>& vector) override;
    std::vector<std::vector<Tile>> deserialize(const std::string& string) const override;

private:
    std::unordered_map<TileType,int> tileTypeToInt;
};
}