#pragma once

#include "Text.h"
#include "VisibilityLayer.h"

namespace graphics
{
struct LayeredText
{
    VisibilityLayer layer;
    Text text;
};

inline bool operator<(const LayeredText& lhs, const LayeredText& rhs)
{
    return lhs.layer > rhs.layer;
}
}