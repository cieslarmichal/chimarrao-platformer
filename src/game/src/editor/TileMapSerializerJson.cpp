#include "TileMapSerializerJson.h"

#include "nlohmann/json.hpp"

namespace game
{

TileMapSerializerJson::TileMapSerializerJson() {

}

std::string TileMapSerializerJson::serialize(const std::vector<std::vector<Tile>>& vector)
{
    nlohmann::json json = nlohmann::json().object();
    for (int y = 0; y < vector.size(); y++)
    {
        for (int x = 0; x < vector[y].size(); x++)
        {
            json[x][y] = tileTypeToInt[vector[y][x].type];
        }
    }
    return json.dump();
}

std::vector<std::vector<Tile>> TileMapSerializerJson::deserialize(const std::string& string) const
{
    return std::vector<std::vector<Tile>>();
}
}