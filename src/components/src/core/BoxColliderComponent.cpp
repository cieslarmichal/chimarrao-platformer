#include "BoxColliderComponent.h"

#include <array>
#include <cmath>

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
    Component::loadDependentComponents();

    movementComponent = owner->getComponent<KeyboardMovementComponent>();
    velocityComponent = owner->getComponent<VelocityComponent>();
}

bool BoxColliderComponent::intersects(const utils::Vector2f& point)
{
    const utils::FloatRect& thisRect = getCollisionBox();
    return thisRect.contains(point);
}

bool BoxColliderComponent::intersectsX(const std::shared_ptr<BoxColliderComponent>& other)
{
    if (other)
    {
        const utils::FloatRect& thisRect = getNextFrameXCollisionBox();
        const utils::FloatRect& otherRect = other->getNextFrameXCollisionBox();

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
        const utils::FloatRect& thisRect = getNextFrameYCollisionBox();
        const utils::FloatRect& otherRect = other->getNextFrameYCollisionBox();

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

    const utils::FloatRect& otherRect = other->getNextFrameXCollisionBox();

    float left = abs(otherRect.left + otherRect.width - nextFrameCollisionBoundaries.left);
    float right =
        abs(otherRect.left - (nextFrameCollisionBoundaries.left + nextFrameCollisionBoundaries.width));

    if (left < right)
    {
        movementComponent->canMoveLeft = false;
    }
    else
    {
        movementComponent->canMoveRight = false;
    }
}

void BoxColliderComponent::resolveOverlapY(const std::shared_ptr<BoxColliderComponent>& other)
{
    if (not movementComponent)
    {
        return;
    }

    const utils::FloatRect& otherRect = other->getNextFrameYCollisionBox();

    float top = abs(otherRect.top + otherRect.height - nextFrameCollisionBoundaries.top);
    float bot = abs(otherRect.top - (nextFrameCollisionBoundaries.top + nextFrameCollisionBoundaries.height));

    if (top < bot)
    {
        movementComponent->canMoveUp = false;
    }
    else
    {
        movementComponent->canMoveDown = false;
    }
}

const sf::FloatRect& BoxColliderComponent::getCollisionBox()
{
    const sf::Vector2f& pos = owner->transform->getPosition();
    collisionBoundaries.left = pos.x + offset.x;
    collisionBoundaries.top = pos.y + offset.y;
    return collisionBoundaries;
}

void BoxColliderComponent::setAvailableMovementDirections()
{
    if (movementComponent)
    {
        movementComponent->canMoveRight = true;
        movementComponent->canMoveLeft = true;
        movementComponent->canMoveUp = true;
        movementComponent->canMoveDown = true;
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