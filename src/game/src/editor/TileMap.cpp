#include "TileMap.h"

#include "GetProjectPath.h"

#include "nlohmann/json.hpp"

namespace game
{

TileMap::TileMap(utils::Vector2i mapSizeInit, utils::Vector2f tileSizeInit)
    : mapSize(mapSizeInit), tileSize(tileSizeInit)
{
    // TODO: calculate map size
    for (int y = 0; y < mapSize.y; y++)
    {
        tiles.emplace_back(mapSize.x, nullptr);
        for (auto& tile : tiles[y])
        {
            tile = std::make_shared<Tile>();
        }
    }
}

void TileMap::saveToFile() {

    for (int y = 0; y < mapSize.y; y++)
    {
        for (int x = 0; x < mapSize.x; x++)
        {

        }
    }
}

std::shared_ptr<Tile>& TileMap::getTile(utils::Vector2i position)
{
    return tiles[position.y][position.x];
}

void TileMap::setTile(utils::Vector2i position, TileType value)
{
    tiles[position.y][position.x]->type = value;
}
void TileMap::setName(const std::string& nameInit)
{
    name = nameInit;
}

const std::string& TileMap::getName() const
{
    return name;
}

std::string TileMap::getPath() const
{
    return utils::getProjectPath("chimarrao-platformer") + "maps/" + name + ".chm";
}
}
