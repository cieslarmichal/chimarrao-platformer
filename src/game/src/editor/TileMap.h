#pragma once

#include <Vector.h>
#include <memory>
#include <vector>

#include "FileAccess.h"
#include "Tile.h"
#include "TileMapInfo.h"
#include "TileMapSerializer.h"
#include "TileType.h"

namespace game
{
// TODO: add editor namespace
class TileMap
{
public:
    TileMap(std::string name, utils::Vector2i mapSize, std::unique_ptr<TileMapSerializer>,
            std::shared_ptr<utils::FileAccess>);

    void saveToFile();
    void loadFromFile(std::string path);
    void setTileMapInfo(const TileMapInfo&);
    void setName(const std::string&);
    const std::string& getName() const;
    std::string getPath() const;
    void setTile(utils::Vector2i position, TileType value);
    std::shared_ptr<Tile>& getTile(utils::Vector2i position);
    utils::Vector2i getSize() const;

private:
    TileMapInfo tileMapInfo;
    std::unique_ptr<TileMapSerializer> tileMapSerializer;
    std::shared_ptr<utils::FileAccess> fileAccess;
};
}