#pragma once

#include <Vector.h>
#include <memory>
#include <vector>

#include "Tile.h"
#include "TileMapInfo.h"
#include "TileType.h"

namespace game
{
// TODO: add editor namespace
class TileMap
{
public:
    TileMap(std::string name, utils::Vector2i mapSize);
    void saveToFile();
    void setName(const std::string&);
    const std::string& getName() const;
    std::string getPath() const;
    void setTile(utils::Vector2i position, TileType value);
    std::shared_ptr<Tile>& getTile(utils::Vector2i position);

private:
    TileMapInfo tileMapInfo;
};
}