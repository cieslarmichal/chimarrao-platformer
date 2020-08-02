#pragma once

#include <memory>

#include "GraphicsFactory.h"

namespace graphics
{
class DefaultGraphicsFactory : public GraphicsFactory
{
public:
    std::unique_ptr<RendererPool>
    createRendererPool(const utils::Vector2u& renderingRegionSize,
                       const utils::Vector2u& logicalRegionSize) const override;
    std::unique_ptr<Window> createWindow(const utils::Vector2u& windowSize, const std::string& title) const override;
};
}
