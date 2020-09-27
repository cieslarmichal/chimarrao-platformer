#pragma once

#include <memory>

#include "GraphicsFactory.h"

namespace graphics
{
class DefaultGraphicsFactory : public GraphicsFactory
{
public:
    std::unique_ptr<RendererPool> createRendererPool(std::shared_ptr<window::Window> window,
                                                     const utils::Vector2u& renderingRegionSize,
                                                     const utils::Vector2u& logicalRegionSize) const override;
};
}
