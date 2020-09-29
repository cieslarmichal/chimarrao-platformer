#pragma once

#include "RectangleShape.h"
#include "VisibilityLayer.h"

namespace graphics
{
struct LayeredShape
{
    VisibilityLayer layer;
    RectangleShape shape;
};

inline bool operator<(const LayeredShape& lhs, const LayeredShape& rhs)
{
    return lhs.layer > rhs.layer;
}
}