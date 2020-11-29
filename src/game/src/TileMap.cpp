#include "TileMap.h"

namespace game
{

TileMap::TileMap(utils::Vector2i mapSizeInit, utils::Vector2f tileSizeInit)
    : mapSize(mapSizeInit), tileSize(tileSizeInit)
{
    // TODO: calculate map size
    for (int x = 0; x < mapSize.y; x++)
    {
        nodes.emplace_back(mapSize.x, 0);
    }
}

void TileMap::saveToFile() {}
void TileMap::setTile(utils::Vector2i position, int value)
{
    nodes[position.y][position.x] = value;
}
int TileMap::getTile(utils::Vector2i position)
{
    return nodes[position.y][position.x];
}
}
