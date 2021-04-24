#pragma once

#include "Rect.h"

namespace components::core
{
struct CollisionInfo
{
    bool colliding = false;
    const utils::FloatRect* other = nullptr;
};
}