#include "DefaultGraphicsFactory.h"

#include "FontStorageSfml.h"
#include "RenderTargetSfml.h"
#include "RendererPoolSfml.h"
#include "TextureStorageSfml.h"

namespace graphics
{

std::unique_ptr<RendererPool>
DefaultGraphicsFactory::createRendererPool(std::shared_ptr<window::Window> window,
                                           const utils::Vector2u& renderingRegionSize,
                                           const utils::Vector2u& logicalRegionSize) const
{
    return std::make_unique<RendererPoolSfml>(
        std::make_unique<RenderTargetSfml>(window, renderingRegionSize, logicalRegionSize),
        std::make_unique<TextureStorageSfml>(), std::make_unique<FontStorageSfml>());
}

}