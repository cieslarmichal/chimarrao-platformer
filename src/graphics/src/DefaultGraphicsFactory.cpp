#include "DefaultGraphicsFactory.h"

#include "RenderTargetSfml.h"
#include "RendererPoolSfml.h"
#include "TextureLoaderSfml.h"
#include "TextureStorageSfml.h"

namespace graphics
{

std::unique_ptr<RendererPool>
DefaultGraphicsFactory::createRendererPool(const utils::Vector2u& renderingRegionSize,
                                           const utils::Vector2u& logicalRegionSize) const
{
    return std::make_unique<RendererPoolSfml>(
        std::make_unique<RenderTargetSfml>(renderingRegionSize, logicalRegionSize),
        std::make_unique<TextureStorageSfml>(std::make_unique<TextureLoaderSfml>()));
}

}