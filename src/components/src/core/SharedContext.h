#pragma once

#include <memory>

#include "RendererPool.h"

namespace components::core
{
struct SharedContext
{
    const std::shared_ptr<window::Window>& window;
    const std::shared_ptr<graphics::RendererPool>& rendererPool;
};
}