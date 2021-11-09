#pragma once

#include <memory>
#include <utility>

#include "RendererPool.h"

namespace components::core
{
struct SharedContext
{
    explicit SharedContext(std::shared_ptr<graphics::RendererPool> rendererPoolInit)
        : rendererPool{std::move(rendererPoolInit)}
    {
    }

    std::shared_ptr<graphics::RendererPool> rendererPool;
};
}