#include "DefaultTileMap.h"

#include <utility>

#include "ProjectPathReader.h"
#include "TileMapSerializerJson.h"

namespace game
{

DefaultTileMap::DefaultTileMap(std::string name, utils::Vector2i mapSizeInit,
                 std::unique_ptr<TileMapSerializer> tileMapSerializerInit,
                 std::shared_ptr<utils::FileAccess> fileAccessInit)
    : tileMapInfo{std::move(name), mapSizeInit},
      tileMapSerializer{std::move(tileMapSerializerInit)},
      fileAccess{std::move(fileAccessInit)}
{
    for (int y = 0; y < tileMapInfo.mapSize.y; y++)
    {
        tileMapInfo.tiles.emplace_back(tileMapInfo.mapSize.x, nullptr);
        for (auto& tile : tileMapInfo.tiles[y])
        {
            tile = std::make_shared<Tile>();
        }
    }
}

void DefaultTileMap::saveToFile()
{
    auto serializedMapInfo = tileMapSerializer->serialize(tileMapInfo);
    fileAccess->write(getPath(), serializedMapInfo);
}

std::shared_ptr<Tile>& DefaultTileMap::getTile(utils::Vector2i position)
{
    return tileMapInfo.tiles[position.y][position.x];
}

void DefaultTileMap::setTile(utils::Vector2i position, TileType value)
{
    tileMapInfo.tiles[position.y][position.x]->type = value;
}

void DefaultTileMap::setName(const std::string& nameInit)
{
    tileMapInfo.name = nameInit;
}

const std::string& DefaultTileMap::getName() const
{
    return tileMapInfo.name;
}

std::string DefaultTileMap::getPath() const
{
    return utils::ProjectPathReader::getProjectRootPath() + "maps/" + tileMapInfo.name + ".map";
}

void DefaultTileMap::setTileMapInfo(const TileMapInfo& newTileMapInfo)
{
    tileMapInfo = newTileMapInfo;
    if (tileMapInfo.tiles.empty())
    {
        for (int y = 0; y < tileMapInfo.mapSize.y; y++)
        {
            tileMapInfo.tiles.emplace_back(tileMapInfo.mapSize.x, nullptr);
            for (auto& tile : tileMapInfo.tiles[y])
            {
                tile = std::make_shared<Tile>();
            }
        }
    }
}

void DefaultTileMap::loadFromFile(const std::string& path)
{
    auto mapFile = fileAccess->readContent(path);
    auto tileMapInfoFroFile = tileMapSerializer->deserialize(mapFile);
    setTileMapInfo(tileMapInfoFroFile);
}

utils::Vector2i DefaultTileMap::getSize() const
{
    return tileMapInfo.mapSize;
}
}
