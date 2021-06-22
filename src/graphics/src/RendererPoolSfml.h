#pragma once

#include <boost/functional/hash.hpp>
#include <memory>
#include <unordered_set>
#include <vector>

#include "ContextRenderer.h"
#include "FontStorage.h"
#include "GraphicsIdGenerator.h"
#include "LayeredShape.h"
#include "LayeredText.h"
#include "RectangleShape.h"
#include "RendererPool.h"
#include "Text.h"
#include "TextureStorage.h"

namespace graphics
{
class RendererPoolSfml : public RendererPool
{
public:
    RendererPoolSfml(std::unique_ptr<ContextRenderer>, std::unique_ptr<TextureStorage>,
                     std::unique_ptr<FontStorage>);

    GraphicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position, const Color&,
                       VisibilityLayer = VisibilityLayer::First, bool = false) override;
    GraphicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position, const TexturePath&,
                       VisibilityLayer = VisibilityLayer::First, bool = false) override;
    GraphicsId acquireText(const utils::Vector2f& position, const std::string& text, const FontPath&,
                           unsigned characterSize, VisibilityLayer = VisibilityLayer::First,
                           const Color& = Color::Black, bool = false) override;
    void release(const GraphicsId&) override;
    void renderAll() override;
    void setPosition(const GraphicsId&, const utils::Vector2f& position) override;
    boost::optional<utils::Vector2f> getPosition(const GraphicsId&) override;
    void setTexture(const GraphicsId&, const TextureRect&, const utils::Vector2f& scale = {1, 1}) override;
    void setText(const GraphicsId&, const std::string& text) override;
    boost::optional<std::string> getText(const GraphicsId&) const override;
    void setVisibility(const GraphicsId&, VisibilityLayer) override;
    void setColor(const GraphicsId&, const Color&) override;
    void setOutline(const GraphicsId&, float thickness, const Color&) override;
    void setRenderingSize(const utils::Vector2u& renderingSize) override;
    void synchronizeRenderingSize() override;
    void setCenter(const utils::Vector2f &) override;
    const utils::Vector2f& getCenter() const override;

private:
    void cleanUnusedShapes();

public:
    const utils::Vector2f& getViewSize() const override;

private:
    std::vector<ShapeRenderingInfo>::const_iterator findLayeredShapePosition(const GraphicsId&) const;
    std::vector<TextRenderingInfo>::const_iterator findLayeredTextPosition(const GraphicsId&) const;

    std::unique_ptr<ContextRenderer> contextRenderer;
    std::unique_ptr<TextureStorage> textureStorage;
    std::unique_ptr<FontStorage> fontStorage;
    std::vector<ShapeRenderingInfo> layeredShapes;
    std::vector<TextRenderingInfo> layeredTexts;
    std::unordered_set<GraphicsId, boost::hash<GraphicsId>> graphicsObjectsToRemove;
};
}
