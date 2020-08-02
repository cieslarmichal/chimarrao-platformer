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
    createRendererPool(const utils::Vector2u& renderingRegionSize,
                         const utils::Vector2u& logicalRegionSize) const = 0;
    virtual std::unique_ptr<Window> createWindow(const utils::Vector2u& windowSize,
                                          const std::string& title) const = 0;

    static std::unique_ptr<GraphicsFactory> createGraphicsFactory();
};
}
