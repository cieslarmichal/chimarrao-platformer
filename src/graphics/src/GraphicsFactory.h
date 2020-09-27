#pragma once

#include <memory>

#include "RendererPool.h"
#include "Window.h"

namespace graphics
{
class GraphicsFactory
{
public:
    virtual ~GraphicsFactory() = default;

    virtual std::unique_ptr<RendererPool>
    createRendererPool(std::shared_ptr<window::Window> window, const utils::Vector2u& renderingRegionSize,
                       const utils::Vector2u& logicalRegionSize) const = 0;

    static std::unique_ptr<GraphicsFactory> createGraphicsFactory();
};
}
