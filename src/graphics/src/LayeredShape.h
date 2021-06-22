#pragma once

#include "RectangleShape.h"
#include "VisibilityLayer.h"

namespace graphics
{
struct ShapeRenderingInfo
{
    VisibilityLayer layer;
    RectangleShape shape;
    bool relativeRendering = false;
};

inline bool operator<(const ShapeRenderingInfo& lhs, const ShapeRenderingInfo& rhs)
{
    return lhs.layer > rhs.layer;
}
}