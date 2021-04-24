#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

#include "StringHelper.h"

namespace components::core
{
enum class CollisionLayer
{
    Default,
    Player,
    Tile
};

inline std::string toString(CollisionLayer collisionLayer)
{
    std::unordered_map<CollisionLayer, std::string> collisionLayerToString{
        {CollisionLayer::Default, "Default"},
        {CollisionLayer::Player, "Player"},
        {CollisionLayer::Tile, "Tile"}};

    return collisionLayerToString.at(collisionLayer);
}

inline CollisionLayer toCollisionLayer(const std::string& collisionLayerAsString)
{
    const auto collisionLayerAsLowerString = utils::StringHelper::getLowerCases(collisionLayerAsString);

    std::unordered_map<std::string, CollisionLayer> stringToCollisionLayer{
        {"default", CollisionLayer::Default},
        {"player", CollisionLayer::Player},
        {"tile", CollisionLayer::Tile}};

    return stringToCollisionLayer.at(collisionLayerAsLowerString);
}

inline std::ostream& operator<<(std::ostream& os, CollisionLayer collisionLayer)
{
    return os << "CollisionLayer::" << toString(collisionLayer);
}
}