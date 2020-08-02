#include "DefaultGraphicsFactory.h"

#include "RenderTargetSfml.h"
#include "RendererPoolSfml.h"
#include "TextureLoaderSfml.h"
#include "TextureStorageSfml.h"
#include "WindowProxySfml.h"
#include "WindowSfml.h"

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

std::unique_ptr<Window> DefaultGraphicsFactory::createWindow(const utils::Vector2u& windowSize,
                                                             const std::string& title) const
{
    return std::make_unique<WindowSfml>(windowSize, title, std::make_unique<WindowProxySfml>());
}

}