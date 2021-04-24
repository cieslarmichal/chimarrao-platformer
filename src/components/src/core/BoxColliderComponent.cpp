#include "BoxColliderComponent.h"

#include "ComponentOwner.h"

namespace components::core
{

BoxColliderComponent::BoxColliderComponent(ComponentOwner* owner, const utils::Vector2f& sizeInit,
                                           const utils::Vector2f& offsetInit)
    : ColliderComponent{owner}, offset{offsetInit}
{
    rect.width = sizeInit.x;
    rect.height = sizeInit.y;
    setPosition();
}

bool BoxColliderComponent::intersects(const utils::Vector2f& point)
{
    const sf::FloatRect& thisRect = getCollisionBox();
    return thisRect.contains(point);
}

CollisionInfo BoxColliderComponent::intersects(std::shared_ptr<ColliderComponent> other)
{
    CollisionInfo collisionInfo;

    std::shared_ptr<BoxColliderComponent> otherBoxCollider =
        std::dynamic_pointer_cast<BoxColliderComponent>(other);

    if (otherBoxCollider)
    {
        const sf::FloatRect& thisRect = getCollisionBox();
        const sf::FloatRect& otherRect = otherBoxCollider->getCollisionBox();

        if (thisRect.intersects(otherRect))
        {
            collisionInfo.colliding = true;
            collisionInfo.other = &otherRect;
        }
    }

    return collisionInfo;
}

void BoxColliderComponent::resolveOverlap(const CollisionInfo& collisionInfo)
{
    if (not collisionInfo.colliding or not collisionInfo.other)
    {
        return;
    }

    if (owner->transform->isStaticTransform())
    {
        return;
    }

    const sf::FloatRect& rect1 = getCollisionBox();
    const sf::FloatRect& rect2 = *collisionInfo.other;

    switch (calculateCollisionSource(rect1, rect2))
    {
    case CollisionSource::Left:
    {
        const auto moveRightDistance = (rect2.left + rect2.width) - rect1.left;
        owner->transform->addPosition(moveRightDistance, 0);
        return;
    }
    case CollisionSource::Right:
    {
        const auto moveLeftDistance = -((rect1.left + rect1.width) - rect2.left);
        owner->transform->addPosition(moveLeftDistance, 0);
        return;
    }
    case CollisionSource::Above:
    {
        const auto moveDownDistance = (rect2.top + rect2.height) - rect1.top;
        owner->transform->addPosition(0, moveDownDistance);
        return;
    }
    case CollisionSource::Below:
    {
        const auto moveUpDistance = -((rect1.top + rect1.height) - rect2.top);
        owner->transform->addPosition(0, moveUpDistance);
        return;
    }
    }
}

const sf::FloatRect& BoxColliderComponent::getCollisionBox()
{
    setPosition();
    return rect;
}

void BoxColliderComponent::setPosition()
{
    const sf::Vector2f& pos = owner->transform->getPosition();

    rect.left = pos.x + offset.x;
    rect.top = pos.y + offset.y;
}

CollisionSource BoxColliderComponent::calculateCollisionSource(const utils::FloatRect& rect1,
                                                               const utils::FloatRect& rect2)
{
    float xDifferenceBetweenCentrePoints =
        (rect1.left + (rect1.width * 0.5f)) - (rect2.left + (rect2.width * 0.5f));
    float yDifferenceBetweenCentrePoints =
        (rect1.top + (rect1.height * 0.5f)) - (rect2.top + (rect2.height * 0.5f));

    if (fabs(xDifferenceBetweenCentrePoints) > fabs(yDifferenceBetweenCentrePoints))
    {
        if (xDifferenceBetweenCentrePoints > 0)
        {
            return CollisionSource::Left;
        }
        else
        {
            return CollisionSource::Right;
        }
    }
    else
    {
        if (yDifferenceBetweenCentrePoints > 0)
        {
            return CollisionSource::Above;
        }
        else
        {
            return CollisionSource::Below;
        }
    }
}

}