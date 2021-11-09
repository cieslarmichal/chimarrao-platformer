#pragma once

#include <memory>

#include "RendererPool.h"

namespace components::core
{
struct SharedContext
{
    std::shared_ptr<graphics::RendererPool> rendererPool;
};
}