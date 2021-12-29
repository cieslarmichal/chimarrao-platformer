#pragma once

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "ProjectPathReader.h"
#include "exceptions/TexturePathForTileNotFound.h"
#include "exceptions/TileTypeNotFound.h"

namespace game
{
enum class TileType
{
    Brick,
    Grass,
    Tree,
    Bush,
    Campfire,
    Chest,
    Player,
    Npc,
    Bandit,
    Rabbit
};

const std::vector<TileType> tileTypes{TileType::Brick,    TileType::Grass, TileType::Tree,   TileType::Bush,
                                      TileType::Campfire, TileType::Chest, TileType::Player, TileType::Npc,
                                      TileType::Bandit,   TileType::Rabbit};

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
        {TileType::Campfire,
         utils::ProjectPathReader::getProjectRootPath() + "resources/campfire_without_animation.png"},
        {TileType::Chest, utils::ProjectPathReader::getProjectRootPath() + "resources/chest.png"},
        {TileType::Player, utils::ProjectPathReader::getProjectRootPath() + "resources/player.png"},
        {TileType::Npc, utils::ProjectPathReader::getProjectRootPath() + "resources/npc.png"},
        {TileType::Bandit, utils::ProjectPathReader::getProjectRootPath() + "resources/bandit.png"},
        {TileType::Rabbit, utils::ProjectPathReader::getProjectRootPath() + "resources/rabbit.png"},
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

inline int tileTypeToInt(std::optional<TileType> type)
{
    const std::unordered_map<std::optional<TileType>, int> tileTypeToInt{
        {std::nullopt, 0},   {TileType::Grass, 1},    {TileType::Brick, 2},  {TileType::Tree, 3},
        {TileType::Bush, 4}, {TileType::Campfire, 5}, {TileType::Chest, 6},  {TileType::Player, 7},
        {TileType::Npc, 8},  {TileType::Bandit, 9},   {TileType::Rabbit, 10}};

    try
    {
        return tileTypeToInt.at(type);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::TileTypeNotFound{e.what()};
    }
}

inline std::optional<TileType> intToTileType(int tileTypeInt)
{
    const std::unordered_map<int, std::optional<TileType>> intToTileType{
        {0, std::nullopt},   {1, TileType::Grass},    {2, TileType::Brick},  {3, TileType::Tree},
        {4, TileType::Bush}, {5, TileType::Campfire}, {6, TileType::Chest},  {7, TileType::Player},
        {8, TileType::Npc},  {9, TileType::Bandit},   {10, TileType::Rabbit}};

    try
    {
        return intToTileType.at(tileTypeInt);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::TileTypeNotFound{e.what()};
    }
}

}