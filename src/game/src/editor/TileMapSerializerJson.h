#pragma once

#include "nlohmann/json_fwd.hpp"

#include "Tile.h"
#include "TileMapSerializer.h"

namespace game
{
class TileMapSerializerJson : public TileMapSerializer
{
public:
    TileMapSerializerJson();

    std::string serialize(const TileMapInfo& tileMapInfo) const override;
    TileMapInfo deserialize(const std::string& string) const override;

private:
    void checkMapName(const std::string&) const;
    void checkMapSize(const utils::Vector2i&, const std::vector<std::vector<std::shared_ptr<Tile>>>&) const;
    TileMapInfo parseTileMap(const nlohmann::json&) const;
    std::string parseMapName(const nlohmann::json&) const;
    utils::Vector2i parseMapSize(const nlohmann::json&) const;
    std::vector<std::vector<std::shared_ptr<Tile>>> parseTiles(const nlohmann::json&, utils::Vector2i) const;
    const std::unordered_map<std::optional<TileType>, int> tileTypeToInt{{std::nullopt, 0}};
    const std::unordered_map<int, std::optional<TileType>> intToTileType{{0, std::nullopt}};
};
}