#include "BoxColliderComponent.h"

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
    const utils::FloatRect& thisRect = getNextFrameCollisionBox();
    return thisRect.contains(point);
}

bool BoxColliderComponent::intersects(const std::shared_ptr<BoxColliderComponent>& other)
{
    if (other)
    {
        const utils::FloatRect& thisRect = getNextFrameCollisionBox();
        const utils::FloatRect& otherRect = other->getNextFrameCollisionBox();

        if (thisRect.intersects(otherRect))
        {
            return true;
        }
    }
    return false;
}

void BoxColliderComponent::resolveOverlap(const std::shared_ptr<BoxColliderComponent>& other)
{
    if (not movementComponent)
    {
        return;
    }

    if (owner->transform->isStaticTransform())
    {
        return;
    }

    const utils::FloatRect& otherRect = other->collisionBoundaries;

    switch (calculateCollisionSource(otherRect))
    {
    case CollisionSource::Left:
    {
        movementComponent->canMoveLeft = false;
        return;
    }
    case CollisionSource::Right:
    {
        movementComponent->canMoveRight = false;
        return;
    }
    case CollisionSource::Above:
    {
        movementComponent->canMoveUp = false;
        return;
    }
    case CollisionSource::Below:
    {
        movementComponent->canMoveDown = false;
        return;
    }
    }
}

const sf::FloatRect& BoxColliderComponent::getCollisionBox()
{
    const sf::Vector2f& pos = owner->transform->getPosition();
    collisionBoundaries.left = pos.x + offset.x;
    collisionBoundaries.top = pos.y + offset.y;
    return collisionBoundaries;
}

const utils::FloatRect& BoxColliderComponent::getNextFrameCollisionBox()
{
    getCollisionBox();

    if (not velocityComponent)
    {
        return collisionBoundaries;
    }
    nextFrameCollisionBoundaries.left =
        collisionBoundaries.left + velocityComponent->getVelocity().x * currentDeltaTime.count();
    nextFrameCollisionBoundaries.top =
        collisionBoundaries.top + velocityComponent->getVelocity().y * currentDeltaTime.count();

    return nextFrameCollisionBoundaries;
}

CollisionSource BoxColliderComponent::calculateCollisionSource(const utils::FloatRect& otherRect)
{
    float distances[4];
    const int TOP = 0;
    const int BOT = 1;
    const int LEFT = 2;
    const int RIGHT = 3;

    distances[TOP] = abs(otherRect.top + otherRect.height - nextFrameCollisionBoundaries.top);
    distances[BOT] = abs(otherRect.top - (nextFrameCollisionBoundaries.top + nextFrameCollisionBoundaries.height));
    distances[LEFT] = abs(otherRect.left + otherRect.width - nextFrameCollisionBoundaries.left);
    distances[RIGHT] = abs(otherRect.left - (nextFrameCollisionBoundaries.left + nextFrameCollisionBoundaries.width));

    if ((distances[TOP] < distances[BOT] && distances[TOP] < distances[LEFT] &&
         distances[TOP] < distances[RIGHT]))
    {
        return CollisionSource::Above;
    }
    else if ((distances[BOT] < distances[TOP] && distances[BOT] < distances[LEFT] &&
              distances[BOT] < distances[RIGHT]))
    {
        return CollisionSource::Below;
    }
    else if ((distances[LEFT] < distances[BOT] && distances[LEFT] < distances[TOP] &&
              distances[LEFT] < distances[RIGHT]))
    {
        return CollisionSource::Left;
    }
    else if ((distances[RIGHT] < distances[BOT] && distances[RIGHT] < distances[LEFT] &&
              distances[RIGHT] < distances[TOP]))
    {
        return CollisionSource::Right;
    }

    return CollisionSource::None;
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

}