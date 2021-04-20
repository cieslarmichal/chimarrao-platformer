#include "TileMap.h"

#include <utility>

#include "ProjectPathReader.h"

namespace game
{

TileMap::TileMap(std::string name, utils::Vector2i mapSizeInit) : tileMapInfo{std::move(name), mapSizeInit}
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

void TileMap::saveToFile() {}

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
    return utils::ProjectPathReader::getProjectRootPath() + "maps/" + tileMapInfo.name + ".chm";
}
}
