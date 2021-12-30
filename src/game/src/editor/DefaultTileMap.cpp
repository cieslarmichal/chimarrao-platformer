#include "DefaultTileMap.h"

#include <utility>

#include "ProjectPathReader.h"

namespace game
{

DefaultTileMap::DefaultTileMap(std::string name, utils::Vector2i mapSizeInit,
                               std::unique_ptr<TileMapSerializer> tileMapSerializerInit,
                               std::shared_ptr<utils::FileAccess> fileAccessInit)
    : tileMapInfo{std::move(name), mapSizeInit, {}},
      tileMapSerializer{std::move(tileMapSerializerInit)},
      fileAccess{std::move(fileAccessInit)}
{
    for (int y = 0; y < tileMapInfo.mapSize.y; y++)
    {
        tileMapInfo.tiles.emplace_back(tileMapInfo.mapSize.x, nullptr);
        for (auto& tile : tileMapInfo.tiles[y])
        {
            tile = std::make_shared<TileInfo>();
        }
    }
}

void DefaultTileMap::saveToFile()
{
    auto serializedMapInfo = tileMapSerializer->serialize(tileMapInfo);
    fileAccess->write(getPath(), serializedMapInfo);
}

std::shared_ptr<TileInfo>& DefaultTileMap::getTile(utils::Vector2i position)
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
                tile = std::make_shared<TileInfo>();
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

void DefaultTileMap::extend()
{
    std::vector<std::vector<std::shared_ptr<TileInfo>>> extendedTiles;

    for (int y = 0; y < tileMapInfo.mapSize.y; y++)
    {
        extendedTiles.emplace_back(tileMapInfo.mapSize.x * 2, nullptr);
        for (auto& tile : extendedTiles[y])
        {
            tile = std::make_shared<TileInfo>();
        }
    }

    for (int y = 0; y < tileMapInfo.mapSize.y; y++)
    {
        for (int x = 0; x < tileMapInfo.mapSize.x; x++)
        {
            extendedTiles[y][x] = tileMapInfo.tiles[y][x];
        }
    }

    tileMapInfo.mapSize.x *= 2;
    tileMapInfo.tiles = extendedTiles;
}
}
