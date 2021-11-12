#include "BoxColliderComponent.h"

#include <cmath>

#include "FollowerComponent.h"
#include "FreeFallMovementComponent.h"
#include "IdleNpcMovementComponent.h"
#include "KeyboardMovementComponent.h"

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

    if (collisionLayerInit == CollisionLayer::Player)
    {
        debugGraphics = owner->addGraphicsComponent(
            owner->sharedContext->rendererPool, size,
            utils::Vector2f{nextFrameCollisionBoundaries.left, nextFrameCollisionBoundaries.top},
            graphics::Color::Transparent, graphics::VisibilityLayer::First, utils::Vector2f{0, 0}, false, false);
        debugGraphics->setOutline(0.1, graphics::Color::Red);
    }
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

    if (not movementComponent)
    {
        movementComponent = owner->getComponent<IdleNpcMovementComponent>();
    }

    if (not movementComponent)
    {
        movementComponent = owner->getComponent<FreeFallMovementComponent>();
    }

    // not loading movement component dependent components because of circular dependency

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
    }
    else
    {
        movementComponent->blockMoveRight();
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

utils::Vector2f BoxColliderComponent::getSize() const
{
    return size;
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

    if (collisionLayer == CollisionLayer::Player)
    {
        debugGraphics->setPosition(
            utils::Vector2f{nextFrameCollisionBoundaries.left, nextFrameCollisionBoundaries.top});
    }

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

    if (collisionLayer == CollisionLayer::Player)
    {
        debugGraphics->setPosition(utils::Vector2f{nextFrameCollisionBoundaries.left, nextFrameCollisionBoundaries.top});
    }

    return nextFrameCollisionBoundaries;
}

}