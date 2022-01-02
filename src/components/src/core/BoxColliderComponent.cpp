#include "BoxColliderComponent.h"

#include <cmath>

#include "EnemyFollowerComponent.h"
#include "FreeFallMovementComponent.h"
#include "FriendFollowerComponent.h"
#include "IdleNpcMovementComponent.h"
#include "KeyboardAnimatedMovementComponent.h"
#include "KeyboardHorizontalMovementComponent.h"

namespace components::core
{

BoxColliderComponent::BoxColliderComponent(ComponentOwner* owner, const utils::Vector2f& sizeInit,
                                           CollisionLayer collisionLayerInit,
                                           const utils::Vector2f& offsetInit)
    : Component{owner}, collisionLayer{collisionLayerInit}, offset{offsetInit}, size{sizeInit}
{
    collisionBoundaries.width = sizeInit.x;
    collisionBoundaries.height = sizeInit.y;
    nextFrameCollisionBoundaries = collisionBoundaries;
}

void BoxColliderComponent::update(utils::DeltaTime deltaTime, const input::Input&)
{
    currentDeltaTime = deltaTime;
}

void BoxColliderComponent::loadDependentComponents()
{
    movementComponent = owner->getComponent<KeyboardAnimatedMovementComponent>();

    if (not movementComponent)
    {
        movementComponent = owner->getComponent<FriendFollowerComponent>();
    }

    if (not movementComponent)
    {
        movementComponent = owner->getComponent<EnemyFollowerComponent>();
    }

    if (not movementComponent)
    {
        movementComponent = owner->getComponent<IdleNpcMovementComponent>();
    }

    if (not movementComponent)
    {
        movementComponent = owner->getComponent<FreeFallMovementComponent>();
    }

    if (not movementComponent)
    {
        movementComponent = owner->getComponent<KeyboardHorizontalMovementComponent>();
    }

    velocityComponent = owner->getComponent<VelocityComponent>();
    if (velocityComponent)
    {
        velocityComponent->loadDependentComponents();
    }
}

bool BoxColliderComponent::intersects(const utils::Vector2f& point)
{
    const auto& thisRect = getCollisionBox();
    return thisRect.contains(point);
}

bool BoxColliderComponent::intersectsX(const std::shared_ptr<BoxColliderComponent>& other)
{
    if (other)
    {
        const auto& thisRect = getNextFrameXCollisionBox();
        const auto& otherRect = other->getNextFrameXCollisionBox();

        if (thisRect.intersects(otherRect))
        {
            return true;
        }
    }
    return false;
}

bool BoxColliderComponent::intersectsY(const std::shared_ptr<BoxColliderComponent>& other)
{
    if (other)
    {
        const auto& thisRect = getNextFrameYCollisionBox();
        const auto& otherRect = other->getNextFrameYCollisionBox();

        if (thisRect.intersects(otherRect))
        {
            return true;
        }
    }
    return false;
}

void BoxColliderComponent::resolveOverlapX(const std::shared_ptr<BoxColliderComponent>& other)
{
    if (not movementComponent)
    {
        return;
    }

    const auto& otherRect = other->getNextFrameXCollisionBox();

    const auto left = std::abs(otherRect.left + otherRect.width - nextFrameCollisionBoundaries.left);
    const auto right =
        std::abs(otherRect.left - (nextFrameCollisionBoundaries.left + nextFrameCollisionBoundaries.width));

    if (left < right)
    {
        movementComponent->blockMoveLeft();
        colliderNamesWithDistancesOnXAxis[other->getOwnerName()] = Direction::Left;
    }
    else
    {
        movementComponent->blockMoveRight();
        colliderNamesWithDistancesOnXAxis[other->getOwnerName()] = Direction::Right;
    }
}

void BoxColliderComponent::resolveOverlapY(const std::shared_ptr<BoxColliderComponent>& other)
{
    if (not movementComponent)
    {
        return;
    }

    const auto& otherRect = other->getNextFrameXCollisionBox();

    const auto top = std::abs(otherRect.top + otherRect.height - nextFrameCollisionBoundaries.top);
    const auto bot =
        std::abs(otherRect.top - (nextFrameCollisionBoundaries.top + nextFrameCollisionBoundaries.height));

    const auto collidesWithOtherColliderOnXAxis =
        colliderNamesWithDistancesOnXAxis.contains(other->getOwnerName());

    if (nextFrameCollisionBoundaries.top + nextFrameCollisionBoundaries.height / 2 <
            other->getOwner().transform->getPosition().y and
        collidesWithOtherColliderOnXAxis)
    {
        if (colliderNamesWithDistancesOnXAxis[other->getOwnerName()] == Direction::Left)
        {
            movementComponent->allowMoveLeft();
        }
        else
        {
            movementComponent->allowMoveRight();
        }
    }

    if (top < bot)
    {
        movementComponent->blockMoveUp();
    }
    else
    {
        movementComponent->blockMoveDown();
    }
}

const sf::FloatRect& BoxColliderComponent::getCollisionBox()
{
    const auto& position = owner->transform->getPosition();
    collisionBoundaries.left = position.x + offset.x;
    collisionBoundaries.top = position.y + offset.y;
    return collisionBoundaries;
}

void BoxColliderComponent::setAvailableMovementDirections()
{
    if (movementComponent)
    {
        colliderNamesWithDistancesOnXAxis.clear();
        movementComponent->allowMoveRight();
        movementComponent->allowMoveLeft();
        movementComponent->allowMoveUp();
        movementComponent->allowMoveDown();
    }
}

utils::Vector2f BoxColliderComponent::getSize() const
{
    return size;
}

utils::Vector2f BoxColliderComponent::getPosition() const
{
    auto position = owner->transform->getPosition();
    position.x += offset.x;
    position.y += offset.y;
    return position;
}

CollisionLayer BoxColliderComponent::getCollisionLayer() const
{
    return collisionLayer;
}

void BoxColliderComponent::setCollisionLayer(CollisionLayer layer)
{
    collisionLayer = layer;
}

const utils::FloatRect& BoxColliderComponent::getNextFrameXCollisionBox()
{
    getCollisionBox();

    if (not velocityComponent)
    {
        return collisionBoundaries;
    }

    nextFrameCollisionBoundaries.left =
        collisionBoundaries.left + velocityComponent->getVelocity().x * currentDeltaTime.count();
    nextFrameCollisionBoundaries.top = collisionBoundaries.top;

    //    if (collisionLayer == CollisionLayer::Player)
    //    {
    //        debugGraphics->setPosition(
    //            utils::Vector2f{nextFrameCollisionBoundaries.left, nextFrameCollisionBoundaries.top});
    //    }

    return nextFrameCollisionBoundaries;
}

const utils::FloatRect& BoxColliderComponent::getNextFrameYCollisionBox()
{
    getCollisionBox();

    if (not velocityComponent)
    {
        return collisionBoundaries;
    }

    nextFrameCollisionBoundaries.left = collisionBoundaries.left;
    nextFrameCollisionBoundaries.top =
        collisionBoundaries.top + velocityComponent->getVelocity().y * currentDeltaTime.count();

    //    if (collisionLayer == CollisionLayer::Player)
    //    {
    //        debugGraphics->setPosition(
    //            utils::Vector2f{nextFrameCollisionBoundaries.left, nextFrameCollisionBoundaries.top});
    //    }

    return nextFrameCollisionBoundaries;
}

}