#pragma once

#include <vector>
#include <Vector.h>

namespace game
{
class TileMap final
{
public:
    TileMap(utils::Vector2i mapSize, utils::Vector2f tileSize);
    void saveToFile();
    void setTile(utils::Vector2i position, int value);
    int getTile(utils::Vector2i position);

private:
    utils::Vector2i mapSize;
    utils::Vector2f tileSize;
    std::vector<std::vector<int>> nodes;
};
}