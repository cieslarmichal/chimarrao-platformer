#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "ProjectPathReader.h"
#include "editor/exceptions/TexturePathForTileNotFound.h"

namespace game
{
enum class TileType
{
    Brick,
    Grass,
    Tree,
    Bush,
    Campfire,
    Chest
};

const std::vector<TileType> tileTypes{
    TileType::Brick, TileType::Grass, TileType::Tree, TileType::Bush, TileType::Campfire, TileType::Chest,
};

const TileType defaultTileType = TileType::Brick;

inline TileType getNextTileType(TileType tileType)
{
    auto currentTileType = std::find(tileTypes.begin(), tileTypes.end(), tileType);
    return (currentTileType + 1 < tileTypes.end() ? *(currentTileType + 1) : tileTypes.front());
}

inline std::string tileTypeToPathTexture(TileType type)
{
    const std::unordered_map<TileType, std::string> tileTextures{
        {TileType::Brick, utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/brick.png"},
        {TileType::Grass, utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/2.png"},
        {TileType::Tree, utils::ProjectPathReader::getProjectRootPath() + "resources/tree.png"},
        {TileType::Bush, utils::ProjectPathReader::getProjectRootPath() + "resources/bush.png"},
        {TileType::Campfire, utils::ProjectPathReader::getProjectRootPath() + "resources/campfire.png"},
        {TileType::Chest, utils::ProjectPathReader::getProjectRootPath() + "resources/chest.png"},
    };

    try
    {
        return tileTextures.at(type);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::TexturePathForTileNotFound{e.what()};
    }
}
}