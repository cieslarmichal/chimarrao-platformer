#pragma once

#include "Text.h"
#include "VisibilityLayer.h"

namespace graphics
{
struct TextRenderingInfo
{
    VisibilityLayer layer;
    Text text;
    bool relativeRendering = false;
};

inline bool operator<(const TextRenderingInfo& lhs, const TextRenderingInfo& rhs)
{
    return lhs.layer > rhs.layer;
}
}