#include "BoxColliderComponent.h"

#include <cmath>

#include "AttackComponent.h"
#include "DirectionComponent.h"
#include "FollowerComponent.h"
#include "KeyboardMovementComponent.h"

namespace components::core
{

BoxColliderComponent::BoxColliderComponent(ComponentOwner* owner, const utils::Vector2f& sizeInit,
                                           CollisionLayer collisionLayerInit,
                                           const utils::Vector2f& offsetInit)
    : Component{owner}, collisionLayer{collisionLayerInit}, offset{offsetInit}
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
    movementComponent = owner->getComponent<KeyboardMovementComponent>();
    if (not movementComponent)
    {
        movementComponent = owner->getComponent<FollowerComponent>();
    }
    velocityComponent = owner->getComponent<VelocityComponent>();
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
    }
    else
    {
        movementComponent->blockMoveRight();
    }

    auto& owner = getOwner();
    auto attackComponent = owner.getComponent<AttackComponent>();
    if (attackComponent && attackComponent->isThereAttemptToAttack())
    {
        std::cout<< "AAAAAAAAAAAA";
        auto directionComponent = owner.getComponent<DirectionComponent>();
        const auto expectedAttackDirection = otherRect.left < nextFrameCollisionBoundaries.left ?
                                             animations::AnimationDirection::Left :
                                             animations::AnimationDirection::Right;

        if (directionComponent->getDirection() == expectedAttackDirection)
        {
            attackComponent->attack(other->getOwner());
        }
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
        movementComponent->allowMoveRight();
        movementComponent->allowMoveLeft();
        movementComponent->allowMoveUp();
        movementComponent->allowMoveDown();
    }
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

    return nextFrameCollisionBoundaries;
}

}