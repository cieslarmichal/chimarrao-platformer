#pragma once

#include "GraphicsApi.h"
#include "GraphicsId.h"

namespace graphics
{
class GraphicsIdGenerator
{
public:
    static GRAPHICS_API GraphicsId generateId();
};
}