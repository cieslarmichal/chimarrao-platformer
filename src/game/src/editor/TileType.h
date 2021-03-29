#pragma once

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "GetProjectPath.h"
#include "TexturePathForTileNotFound.h"

namespace game
{
enum class TileType
{
    Brick,
    Grass
};

const std::vector<TileType> tileTypes {TileType::Brick, TileType::Grass, };
const TileType defaultTileType = TileType::Brick;

inline TileType getNextTileType(TileType tileType)
{
    auto currentTileType = std::find(tileTypes.begin(),tileTypes.end(), tileType);
    return (currentTileType + 1 < tileTypes.end() ? *(currentTileType + 1) : tileTypes.front());
}

inline std::string tileTypeToPathTexture(TileType type)
{
    const std::unordered_map<TileType,std::string> tileTextures {
        {TileType::Brick, utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png"},
        {TileType::Grass, utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/2.png"},
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