#include "TileMapSerializerJson.h"

#include "exceptions/InvalidField.h"
#include "exceptions/InvalidMapName.h"
#include "exceptions/InvalidMapRepresentation.h"
#include "exceptions/RequiredFieldNotFound.h"
#include "exceptions/WrongMapSize.h"

#include "nlohmann/json.hpp"

namespace game
{

std::string TileMapSerializerJson::serialize(const TileMapInfo& tileMapInfo) const
{
    nlohmann::json json;
    checkMapName(tileMapInfo.name);
    json["info"]["name"] = tileMapInfo.name;
    checkMapSize(tileMapInfo.mapSize, tileMapInfo.tiles);
    json["info"]["mapSize"]["x"] = tileMapInfo.mapSize.x;
    json["info"]["mapSize"]["y"] = tileMapInfo.mapSize.y;
    for (int y = 0; y < tileMapInfo.tiles.size(); y++)
    {
        for (int x = 0; x < tileMapInfo.tiles[y].size(); x++)
        {
            json["data"]["tiles"][x][y] = tileTypeToInt.at(tileMapInfo.tiles[y][x]->type);
        }
    }
    return json.dump();
}

TileMapInfo TileMapSerializerJson::deserialize(const std::string& textMapRepresentation) const
{
    nlohmann::json jsonMap;
    try
    {
        jsonMap = nlohmann::json::parse(textMapRepresentation);
    }
    catch (...)
    {
        throw exceptions::InvalidMapRepresentation("TileMapSerializerJson: text is not JSON");
    }
    return parseTileMap(jsonMap);
}

void TileMapSerializerJson::checkMapName(const std::string& name) const
{
    if (name.empty())
    {
        throw exceptions::InvalidMapName("TileMapSerializerJson: Map has no name");
    }
}
void TileMapSerializerJson::checkMapSize(const utils::Vector2i& size,
                                         const std::vector<std::vector<std::shared_ptr<Tile>>>& map) const
{
    if (size.x != map.size())
    {
        throw exceptions::WrongMapSize{"TileMapSerializerJson: Wrong map size"};
    }
    else
    {
        for (const auto& row : map)
        {
            if (size.y != row.size())
            {
                throw exceptions::WrongMapSize{"TileMapSerializerJson: Wrong map size"};
            }
        }
    }
}
TileMapInfo TileMapSerializerJson::parseTileMap(const nlohmann::json& mapRepresentationJson) const
{
    TileMapInfo tileMapInfo{};
    if (not mapRepresentationJson.contains("info"))
    {
        throw exceptions::RequiredFieldNotFound{
            R"(TileMapSerializerJson: Field "info" not found while deserialization)"};
    }
    tileMapInfo.name = parseMapName(mapRepresentationJson["info"]);
    tileMapInfo.mapSize = parseMapSiz(mapRepresentationJson["info"]);
    if (not mapRepresentationJson.contains("data"))
    {
        throw exceptions::RequiredFieldNotFound{
            R"(TileMapSerializerJson: Field "data" not found while deserialization)"};
    }
    tileMapInfo.tiles = parseTiles(mapRepresentationJson["data"], tileMapInfo.mapSize);
    return tileMapInfo;
}
std::string TileMapSerializerJson::parseMapName(const nlohmann::json& mapInfoJson) const
{
    if (not mapInfoJson.contains("name"))
    {
        throw exceptions::RequiredFieldNotFound{
            R"(TileMapSerializerJson: Field "name" not found while deserialization)"};
    }
    try
    {
        return mapInfoJson["name"].get<std::string>();
    }
    catch (...)
    {
        throw exceptions::InvalidField{R"(TileMapSerializerJson: Field "name" is not a string)"};
    }
}
utils::Vector2i TileMapSerializerJson::parseMapSiz(const nlohmann::json& mapInfoJson) const
{
    if (not mapInfoJson.contains("mapSize"))
    {
        throw exceptions::RequiredFieldNotFound{
            R"(TileMapSerializerJson: Field "mapSize" not found while deserialization)"};
    }
    auto mapSizeJson = mapInfoJson["mapSize"];
    if (not mapSizeJson.contains("x") or not mapSizeJson.contains("y"))
    {
        throw exceptions::RequiredFieldNotFound{R"(TileMapSerializerJson: Field "mapSize" is incomplete)"};
    }
    try
    {
        return utils::Vector2i{mapSizeJson["x"].get<int>(),
                               mapSizeJson["y"].get<int>()};
    }
    catch (...)
    {
        throw exceptions::InvalidField{R"(TileMapSerializerJson: Field "mapSize""x" or "mapSize""y" is not an integer)"};
    }
}
std::vector<std::vector<std::shared_ptr<Tile>>>
TileMapSerializerJson::parseTiles(const nlohmann::json& mapDataJson, utils::Vector2i size) const
{
    if (not mapDataJson.contains("tiles"))
    {
        throw exceptions::RequiredFieldNotFound{
            R"(TileMapSerializerJson: Field "tiles" not found while deserialization)"};
    }
    auto tilesJson = mapDataJson["tiles"];
    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
    tiles.resize(size.x);
    for (auto& row : tiles)
    {
        row.resize(size.y);
    }

    if (not tilesJson.is_array())
    {
        throw exceptions::InvalidField{
            R"(TileMapSerializerJson: Field "tiles" is not an array or has wrong size)"};
    }
    if (tilesJson.size() != size.x)
    {
        throw exceptions::InvalidField{
            R"(TileMapSerializerJson: Field "tiles" has wrong size)"};
    }
    for (int xIter = 0; xIter < size.x; ++xIter)
    {
        if (not tilesJson[xIter].is_array())
        {
            throw exceptions::InvalidField{
                R"(TileMapSerializerJson: Field "tiles" is not an array or has wrong size)"};
        }
        if (tilesJson.size() != size.y)
        {
            throw exceptions::InvalidField{
                R"(TileMapSerializerJson: Field "tiles" has wrong size)"};
        }
        auto rowJson = tilesJson[xIter];
        for (int yIter = 0; yIter < size.x; ++yIter)
        {
            tiles[xIter][yIter] = std::make_shared<game::Tile>(game::Tile{intToTileType.at(rowJson[yIter].get<int>())});
        }
    }
    return tiles;
}
}