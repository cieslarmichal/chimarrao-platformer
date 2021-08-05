#pragma once

#include "core/ComponentOwner.h"
#include "Quadtree.h"

namespace physics
{
struct RayCastResult
{
    components::core::ComponentOwner* collision;
};

class RayCast
{
public:
    explicit RayCast(Quadtree&);

    RayCastResult cast(const utils::Vector2f& from, const utils::Vector2f& to, unsigned int exclusionID = -1);

private:
    sf::FloatRect buildRect(const utils::Vector2f& lineOne, const utils::Vector2f& lineTwo);
    std::vector<sf::Vector2f> buildLinePoints(const utils::Vector2f& from, const utils::Vector2f& to);

    Quadtree& collisions;
};
}
