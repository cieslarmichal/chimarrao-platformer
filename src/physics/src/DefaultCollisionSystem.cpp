#include "DefaultCollisionSystem.h"

#include "core/FollowerComponent.h"
#include "core/KeyboardMovementComponent.h"

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
    playerCollisions.setBit(toInt(CollisionLayer::Player));
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

    for (const auto& [_, collidersInCollisionLayer] : collidersPerLayers)
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
    for (const auto& [collisionLayer, collidersInCollisionLayer] : collidersPerLayers)
    {
        if (possibleCollisionsInLayers[collisionLayer].getMask() == 0)
        {
            continue;
        }

        for (const auto& collider : collidersInCollisionLayer)
        {
            if (not collider->getOwner().getComponent<components::core::KeyboardMovementComponent>() and
                not collider->getOwner().getComponent<components::core::FollowerComponent>())
            {
                continue;
            }

            collider->setAvailableMovementDirections();

            const auto xCollisions =
                collisionTree.getCollidersIntersectingWithAreaFromX(collider->getNextFrameXCollisionBox());

            for (const auto& collision : xCollisions)
            {
                if (collider->getOwnerId() == collision->getOwnerId())
                {
                    continue;
                }

                const auto layersCollide = possibleCollisionsInLayers[collider->getCollisionLayer()].isBitSet(
                    (toInt(collision->getCollisionLayer())));

                if (layersCollide)
                {
                    if (const auto otherRect = collider->intersectsX(collision); otherRect)
                    {
                        collider->resolveOverlapX(otherRect.value());
                    }
                }
            }

            const auto yCollisions =
                collisionTree.getCollidersIntersectingWithAreaFromY(collider->getNextFrameYCollisionBox());

            for (const auto& collision : yCollisions)
            {
                if (collider->getOwnerId() == collision->getOwnerId())
                {
                    continue;
                }

                const auto layersCollide = possibleCollisionsInLayers[collider->getCollisionLayer()].isBitSet(
                    (toInt(collision->getCollisionLayer())));

                if (layersCollide)
                {
                    if (const auto otherRect = collider->intersectsY(collision); otherRect)
                    {
                        collider->resolveOverlapY(otherRect.value());
                    }
                }
            }
        }
    }
}

}