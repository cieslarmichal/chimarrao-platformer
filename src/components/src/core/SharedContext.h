#pragma once

#include <memory>
#include <utility>

#include "RendererPool.h"

namespace components::core
{
struct SharedContext
{
    explicit SharedContext(const std::shared_ptr<graphics::RendererPool>& rendererPoolInit)
        : rendererPool{rendererPoolInit}
    {
    }

    const std::shared_ptr<graphics::RendererPool>& rendererPool;
};
}