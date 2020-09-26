#pragma once

#include <boost/optional.hpp>
#include <string>

#include "Color.h"
#include "FontPath.h"
#include "GraphicsId.h"
#include "TexturePath.h"
#include "Vector.h"
#include "VisibilityLayer.h"

namespace graphics
{
class RendererPool
{
public:
    virtual ~RendererPool() = default;

    virtual GraphicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position,
                               const Color&,
                               VisibilityLayer = VisibilityLayer::First) = 0;
    virtual GraphicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position,
                               const TexturePath&, VisibilityLayer = VisibilityLayer::First) = 0;
    virtual GraphicsId acquireText(const utils::Vector2f& position, const std::string& text, const FontPath&,
                                   unsigned characterSize = 5, const Color& = Color::Black) = 0;
    virtual void release(const GraphicsId&) = 0;
    virtual void renderAll() = 0;
    virtual void setPosition(const GraphicsId&, const utils::Vector2f& position) = 0;
    virtual boost::optional<utils::Vector2f> getPosition(const GraphicsId&) = 0;
    // TODO: remove scale
    virtual void setTexture(const GraphicsId&, const TexturePath&, const utils::Vector2f& scale = {1, 1}) = 0;
    virtual void setColor(const GraphicsId&, const Color&) = 0;
    virtual void setRenderingSize(const utils::Vector2u&) = 0;
};
}