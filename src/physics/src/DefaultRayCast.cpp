#include "DefaultRayCast.h"

#include <cmath>

#include "core/BoxColliderComponent.h"

namespace physics
{

DefaultRayCast::DefaultRayCast(std::shared_ptr<DefaultQuadtree> quadtree) : collisions{std::move(quadtree)} {}

RayCastResult DefaultRayCast::cast(const utils::Vector2f& from, const utils::Vector2f& to, unsigned int exclusionID,
                            float lineWidth) const
{
    if (from == to)
    {
        return {};
    }

    const auto collisionArea = buildRect(from, to, lineWidth);
    const auto colliders = collisions->getCollidersIntersectingWithAreaFromX(collisionArea);

    if (colliders.empty())
    {
        return {};
    }

    std::vector<sf::Vector2f> linePoints = buildLinePoints(from, to);

    for (const auto& linePoint : linePoints)
    {
        for (const auto& collider : colliders)
        {
            if (exclusionID == collider->getOwner().getId())
            {
                continue;
            }

            const auto entityRect = collider->getCollisionBox();
            if (entityRect.contains(linePoint))
            {
                RayCastResult result{};
                result.collision = &(collider->getOwner());
                return result;
            }
        }
    }

    return {};
}

sf::FloatRect DefaultRayCast::buildRect(const utils::Vector2f& lineOne, const utils::Vector2f& lineTwo,
                                 float lineWidth) const
{
    const float rectWidth = lineWidth;
    const float halfWidth = rectWidth * 0.5f;

    float left, top, width, height;

    if (lineOne.x == lineTwo.x)
    {
        left = lineOne.x - halfWidth;
        top = (lineOne.y < lineTwo.y) ? lineOne.y : lineTwo.y;
        width = rectWidth;
        height = fabsf(lineOne.y - lineTwo.y);
    }
    else
    {
        left = (lineOne.x < lineTwo.x) ? lineOne.x : lineTwo.x;
        top = lineOne.y + halfWidth;
        width = fabsf(lineOne.x - lineTwo.x);
        height = rectWidth;
    }

    return sf::FloatRect(left, top, width, height);
}

std::vector<sf::Vector2f> DefaultRayCast::buildLinePoints(const utils::Vector2f& from,
                                                   const utils::Vector2f& to) const
{
    std::vector<sf::Vector2f> result;

    sf::Vector2f diff = to - from;
    int steps = 0;

    float absDiffX = abs(diff.x);
    float absDiffY = abs(diff.y);

    if (absDiffX > absDiffY)
    {
        steps = absDiffX;
    }
    else
    {
        steps = absDiffY;
    }

    float xStep = diff.x / (float)steps;
    float yStep = diff.y / (float)steps;

    float newX = from.x;
    float newY = from.y;

    for (int i = 0; i <= steps; i++)
    {
        result.emplace_back(newX, newY);

        newX += xStep;
        newY += yStep;
    }

    return result;
}
}