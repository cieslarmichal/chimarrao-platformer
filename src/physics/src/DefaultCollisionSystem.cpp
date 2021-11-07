#include "DefaultCollisionSystem.h"

#include "core/FollowerComponent.h"
#include "core/FreeFallMovementComponent.h"
#include "core/IdleNpcMovementComponent.h"
#include "core/KeyboardMovementComponent.h"

namespace physics
{
using namespace components::core;

DefaultCollisionSystem::DefaultCollisionSystem(std::shared_ptr<Quadtree> quadtree)
    : collisionTree{std::move(quadtree)}
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
    collisionTree->clearAllColliders();

    for (const auto& [_, collidersInCollisionLayer] : collidersPerLayers)
    {
        for (const auto& colliderInCollisionLayer : collidersInCollisionLayer)
        {
            collisionTree->insertCollider(colliderInCollisionLayer);
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
                not collider->getOwner().getComponent<components::core::FollowerComponent>() and
                not collider->getOwner().getComponent<components::core::IdleNpcMovementComponent>() and
                not collider->getOwner().getComponent<components::core::FreeFallMovementComponent>())
            {
                continue;
            }

            if (not collider->isEnabled())
            {
                continue;
            }

            collider->setAvailableMovementDirections();

            const auto xCollisions =
                collisionTree->getCollidersIntersectingWithAreaFromX(collider->getNextFrameXCollisionBox());

            for (const auto& xCollision : xCollisions)
            {
                if (collider->getOwnerId() == xCollision->getOwnerId() or not xCollision->isEnabled())
                {
                    continue;
                }

                const auto layersCollide = possibleCollisionsInLayers[collider->getCollisionLayer()].isBitSet(
                    (toInt(xCollision->getCollisionLayer())));

                if (layersCollide)
                {
                    if (collider->intersectsX(xCollision))
                    {
                        collider->resolveOverlapX(xCollision);
                    }
                }
            }

            const auto yCollisions =
                collisionTree->getCollidersIntersectingWithAreaFromY(collider->getNextFrameYCollisionBox());

            for (const auto& yCollision : yCollisions)
            {
                if (collider->getOwnerId() == yCollision->getOwnerId() or not yCollision->isEnabled())
                {
                    continue;
                }

                const auto layersCollide = possibleCollisionsInLayers[collider->getCollisionLayer()].isBitSet(
                    (toInt(yCollision->getCollisionLayer())));

                if (layersCollide)
                {
                    if (collider->intersectsY(yCollision))
                    {
                        collider->resolveOverlapY(yCollision);
                    }
                }
            }
        }
    }
}

}