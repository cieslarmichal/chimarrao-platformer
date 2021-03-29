#pragma once

#include <Vector.h>
#include <memory>
#include <vector>

#include "Tile.h"
#include "TileType.h"

namespace game
{
class TileMap
{
public:
    TileMap(utils::Vector2i mapSize, utils::Vector2f tileSize);
    void saveToFile();
    void setName(const std::string&);
    const std::string& getName() const;
    std::string getPath() const;
    void setTile(utils::Vector2i position, TileType value);
    std::shared_ptr<Tile>& getTile(utils::Vector2i position);

private:
    std::string name;
    utils::Vector2i mapSize;
    utils::Vector2f tileSize;

    std::vector<std::vector<std::shared_ptr<Tile>>> tiles;
};
}