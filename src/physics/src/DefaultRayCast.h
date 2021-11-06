#pragma once

#include "DefaultQuadtree.h"
#include "core/ComponentOwner.h"

namespace physics
{
struct RayCastResult
{
    components::core::ComponentOwner* collision;
};

class DefaultRayCast
{
public:
    explicit DefaultRayCast(std::shared_ptr<DefaultQuadtree>);

    RayCastResult cast(const utils::Vector2f& from, const utils::Vector2f& to, unsigned int exclusionID = -1,
                       float lineWidth = 0.2f) const;

private:
    sf::FloatRect buildRect(const utils::Vector2f& lineOne, const utils::Vector2f& lineTwo,
                            float lineWidth) const;
    std::vector<sf::Vector2f> buildLinePoints(const utils::Vector2f& from, const utils::Vector2f& to) const;

    std::shared_ptr<DefaultQuadtree> collisions;
};
}
