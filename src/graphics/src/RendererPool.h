#pragma once

#include <boost/optional.hpp>
#include <string>

#include "Color.h"
#include "FontPath.h"
#include "GraphicsId.h"
#include "TexturePath.h"
#include "TextureRect.h"
#include "Vector.h"
#include "VisibilityLayer.h"

namespace graphics
{
class RendererPool
{
public:
    virtual ~RendererPool() = default;

    virtual GraphicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position, const Color&,
                               VisibilityLayer = VisibilityLayer::First, bool = false) = 0;
    virtual GraphicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position,
                               const TexturePath&, VisibilityLayer = VisibilityLayer::First,
                               bool = false) = 0;
    virtual GraphicsId acquireText(const utils::Vector2f& position, const std::string& text, const FontPath&,
                                   unsigned characterSize = 13, VisibilityLayer = VisibilityLayer::First,
                                   const Color& = Color::Black, bool = false) = 0;
    virtual void release(const GraphicsId&) = 0;
    virtual void renderAll() = 0;
    virtual void setPosition(const GraphicsId&, const utils::Vector2f& position) = 0;
    virtual boost::optional<utils::Vector2f> getPosition(const GraphicsId&) = 0;
    virtual void setTexture(const GraphicsId&, const TextureRect&, const utils::Vector2f& scale = {1, 1}) = 0;
    virtual void setText(const GraphicsId&, const std::string& text) = 0;
    virtual boost::optional<std::string> getText(const GraphicsId&) const = 0;
    virtual void setVisibility(const GraphicsId&, VisibilityLayer) = 0;
    virtual void setColor(const GraphicsId&, const Color&) = 0;
    virtual void setOutline(const GraphicsId&, float thickness, const Color&) = 0;
    virtual void setRenderingSize(const utils::Vector2u&) = 0;
    virtual void synchronizeRenderingSize() = 0;
    virtual void setCenter(const utils::Vector2f&) = 0;
    virtual void setSize(const GraphicsId&, const utils::Vector2f&) = 0;
    virtual utils::Vector2f getSize(const GraphicsId&) const = 0;
    virtual const utils::Vector2f& getCenter() const = 0;
    virtual const utils::Vector2f& getViewSize() const = 0;
};
}