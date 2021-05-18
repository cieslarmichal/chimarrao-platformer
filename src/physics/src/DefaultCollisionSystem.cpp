#include "DefaultCollisionSystem.h"

namespace physics
{
using namespace components::core;

DefaultCollisionSystem::DefaultCollisionSystem()
{
    utils::Bitmask defaultCollisions{};
    defaultCollisions.setBit(toInt(CollisionLayer::Default));
    possibleCollisionsInLayers.insert(std::make_pair(CollisionLayer::Default, defaultCollisions));

    const utils::Bitmask tileCollisions{};
    possibleCollisionsInLayers.insert(std::make_pair(CollisionLayer::Tile, tileCollisions));

    utils::Bitmask playerCollisions{};
    playerCollisions.setBit(toInt(CollisionLayer::Default));
    playerCollisions.setBit(toInt(CollisionLayer::Tile));
    possibleCollisionsInLayers.insert(std::make_pair(CollisionLayer::Player, playerCollisions));
}

void DefaultCollisionSystem::add(std::vector<std::shared_ptr<components::core::ComponentOwner>>& owners)
{
    for (auto& owner : owners)
    {
        if (auto collider = owner->getComponent<BoxColliderComponent>())
        {
            const auto collisionLayer = collider->getCollisionLayer();

            if (collidersPerLayers.contains(collisionLayer))
            {
                collidersPerLayers[collisionLayer].push_back(collider);
            }
            else
            {
                collidersPerLayers.insert({collisionLayer, {collider}});
            }
        }
    }
}

void DefaultCollisionSystem::processRemovals()
{
    for (auto& [_, collidersInCollisionLayer] : collidersPerLayers)
    {
        collidersInCollisionLayer.erase(
            std::remove_if(collidersInCollisionLayer.begin(), collidersInCollisionLayer.end(),
                           [](auto& collider) { return collider->shouldBeRemoved(); }),
            collidersInCollisionLayer.end());
    }
}

void DefaultCollisionSystem::update()
{
    collisionTree.clearAllColliders();

    for (auto& [_, collidersInCollisionLayer] : collidersPerLayers)
    {
        for (const auto& colliderInCollisionLayer : collidersInCollisionLayer)
        {
            collisionTree.insertCollider(colliderInCollisionLayer);
        }
    }

    resolve();
}

void DefaultCollisionSystem::resolve()
{
    for (auto& [collisionLayer, collidersInCollisionLayer] : collidersPerLayers)
    {
        if (possibleCollisionsInLayers[collisionLayer].getMask() == 0)
        {
            continue;
        }

        for (auto& collider : collidersInCollisionLayer)
        {
            if (collider->getOwner().transform->isStaticTransform())
            {
                continue;
            }

            auto collisions = collisionTree.getCollidersIntersectingWithArea(collider->getNextFrameCollisionBox());
            collider->setAvailableMovementDirections();
            for (const auto& collision : collisions)
            {
                if (collider->getOwnerId() == collision->getOwnerId())
                {
                    continue;
                }

                bool layersCollide = possibleCollisionsInLayers[collider->getCollisionLayer()].isBitSet(
                    (toInt(collision->getCollisionLayer())));

                if (layersCollide)
                {
                    if (collider->intersects(collision))
                    {
                        collider->resolveOverlap(collision);
                    }
                }
            }
        }
    }
}

}