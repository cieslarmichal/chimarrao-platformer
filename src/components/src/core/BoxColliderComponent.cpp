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

    if (owner->transform->isStaticTransform())
    {
        return;
    }

    const utils::FloatRect& otherRect = other->collisionBoundaries;

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

    if (owner->transform->isStaticTransform())
    {
        return;
    }

    const utils::FloatRect& otherRect = other->collisionBoundaries;

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

CollisionSource BoxColliderComponent::calculateCollisionSource(const utils::FloatRect& otherRect)
{
    std::array<float, 4> distances{};
    constexpr int TOP = 0;
    constexpr int BOT = 1;
    constexpr int LEFT = 2;
    constexpr int RIGHT = 3;

    distances[TOP] = abs(otherRect.top + otherRect.height - nextFrameCollisionBoundaries.top);
    distances[BOT] =
        abs(otherRect.top - (nextFrameCollisionBoundaries.top + nextFrameCollisionBoundaries.height));
    distances[LEFT] = abs(otherRect.left + otherRect.width - nextFrameCollisionBoundaries.left);
    distances[RIGHT] =
        abs(otherRect.left - (nextFrameCollisionBoundaries.left + nextFrameCollisionBoundaries.width));

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