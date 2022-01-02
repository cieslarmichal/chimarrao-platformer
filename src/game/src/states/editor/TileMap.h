#pragma once

#include <Vector.h>
#include <memory>
#include <vector>

#include "FileAccess.h"
#include "TileInfo.h"
#include "TileMapInfo.h"
#include "TileMapSerializer.h"
#include "TileType.h"

namespace game
{
class TileMap
{
public:
    virtual ~TileMap() = default;

    virtual void saveToFile() = 0;
    virtual void loadFromFile(const std::string& path) = 0;
    virtual void setTileMapInfo(const TileMapInfo&) = 0;
    virtual void setName(const std::string&) = 0;
    virtual const std::string& getName() const = 0;
    virtual void setTile(utils::Vector2i position, TileType value) = 0;
    virtual std::shared_ptr<TileInfo>& getTile(utils::Vector2i position) = 0;
    virtual utils::Vector2i getSize() const = 0;
    virtual void extend() = 0;
    virtual bool isCustomMap() const = 0;
};
}