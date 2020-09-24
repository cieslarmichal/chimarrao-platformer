#pragma once

#include <memory>

#include "RendererPool.h"

namespace graphics
{
class GraphicsFactory
{
public:
    virtual ~GraphicsFactory() = default;

    virtual std::unique_ptr<RendererPool>
    createRendererPool(const utils::Vector2u& renderingRegionSize,
                       const utils::Vector2u& logicalRegionSize) const = 0;

    static std::unique_ptr<GraphicsFactory> createGraphicsFactory();
};
}
