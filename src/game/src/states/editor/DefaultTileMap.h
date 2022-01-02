#pragma once

#include <Vector.h>
#include <memory>
#include <vector>

#include "FileAccess.h"
#include "TileInfo.h"
#include "TileMap.h"
#include "TileMapInfo.h"
#include "TileMapSerializer.h"
#include "TileType.h"

namespace game
{
class DefaultTileMap : public TileMap
{
public:
    DefaultTileMap(std::string name, utils::Vector2i mapSize, std::unique_ptr<TileMapSerializer>,
                   std::shared_ptr<utils::FileAccess>);

    void saveToFile() override;
    void loadFromFile(const std::string& path) override;
    void setTileMapInfo(const TileMapInfo&) override;
    void setName(const std::string&) override;
    const std::string& getName() const override;
    void setTile(utils::Vector2i position, TileType value) override;
    std::shared_ptr<TileInfo>& getTile(utils::Vector2i position) override;
    utils::Vector2i getSize() const override;
    void extend() override;
    bool isCustomMap() const override;

private:
    TileMapInfo tileMapInfo;
    std::unique_ptr<TileMapSerializer> tileMapSerializer;
    std::shared_ptr<utils::FileAccess> fileAccess;
    bool customMap{true};
};
}