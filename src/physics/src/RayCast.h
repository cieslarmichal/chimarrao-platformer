#pragma once

#include "core/ComponentOwner.h"

namespace physics
{
struct RayCastResult
{
    components::core::ComponentOwner* collision;
};

class RayCast
{
public:
    virtual ~RayCast() = default;

    virtual RayCastResult cast(const utils::Vector2f& from, const utils::Vector2f& to,
                               unsigned int exclusionID = -1, float lineWidth = 0.2f) const = 0;
};
}