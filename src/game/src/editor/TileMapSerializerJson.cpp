#include "TileMapSerializerJson.h"

#include "nlohmann/json.hpp"

namespace game
{

TileMapSerializerJson::TileMapSerializerJson() {}

std::string TileMapSerializerJson::serialize(const TileMapInfo&)
{
    nlohmann::json json = nlohmann::json().object();
    //    json["info"]["name"] = tileMapInfo.name;
    //    json["info"]["mapSize"]["x"] = tileMapInfo.mapSize.x;
    //    json["info"]["mapSize"]["y"] = tileMapInfo.mapSize.y;
    //    for (int y = 0; y < tileMapInfo.tiles.size(); y++)
    //    {
    //        for (int x = 0; x < tileMapInfo.tiles[y].size(); x++)
    //        {
    //            json["data"]["tiles"][x][y] = tileTypeToInt[tileMapInfo.tiles[y][x]->type];
    //        }
    //    }
    //    return json.dump();
    return "";
}

TileMapInfo TileMapSerializerJson::deserialize(const std::string&) const
{
    return TileMapInfo{};
}
}