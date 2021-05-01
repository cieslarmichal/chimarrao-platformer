#include "TileMap.h"

#include <utility>

#include "ProjectPathReader.h"
#include "TileMapSerializerJson.h"

namespace game
{

TileMap::TileMap(std::string name, utils::Vector2i mapSizeInit,
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

void TileMap::saveToFile()
{
    auto serializedMapInfo = tileMapSerializer->serialize(tileMapInfo);
    fileAccess->write(getPath(), serializedMapInfo);
}

std::shared_ptr<Tile>& TileMap::getTile(utils::Vector2i position)
{
    return tileMapInfo.tiles[position.y][position.x];
}

void TileMap::setTile(utils::Vector2i position, TileType value)
{
    tileMapInfo.tiles[position.y][position.x]->type = value;
}

void TileMap::setName(const std::string& nameInit)
{
    tileMapInfo.name = nameInit;
}

const std::string& TileMap::getName() const
{
    return tileMapInfo.name;
}

std::string TileMap::getPath() const
{
    return utils::ProjectPathReader::getProjectRootPath() + "maps/" + tileMapInfo.name + ".map";
}
void TileMap::setTileMapInfo(const TileMapInfo& newTileMapInfo)
{
    tileMapInfo = newTileMapInfo;
    if (tileMapInfo.tiles.empty())
    {
        for (int y = 0; y < tileMapInfo.mapSize.x; y++)
        {
            tileMapInfo.tiles.emplace_back(tileMapInfo.mapSize.x, nullptr);
            for (auto& tile : tileMapInfo.tiles[y])
            {
                tile = std::make_shared<Tile>();
            }
        }
    }
}
}
