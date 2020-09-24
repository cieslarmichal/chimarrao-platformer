#pragma once

#include <boost/functional/hash.hpp>
#include <memory>
#include <unordered_set>
#include <vector>

#include "ContextRenderer.h"
#include "FontStorage.h"
#include "GraphicsIdGenerator.h"
#include "Layer.h"
#include "RectangleShape.h"
#include "RendererPool.h"
#include "Text.h"
#include "TextureStorage.h"

namespace graphics
{
struct LayeredShape
{
    Layer layer;
    RectangleShape shape;
};

inline bool operator<(const LayeredShape& lhs, const LayeredShape& rhs)
{
    return lhs.layer < rhs.layer;
}

class RendererPoolSfml : public RendererPool
{
public:
    RendererPoolSfml(std::unique_ptr<ContextRenderer>, std::unique_ptr<TextureStorage>,
                     std::unique_ptr<FontStorage>);

    GraphicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position, const Color&, Layer = Layer::First) override;
    GraphicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position,
                       const TexturePath&, Layer = Layer::First) override;
    GraphicsId acquireText(const utils::Vector2f& position, const std::string& text, const FontPath&,
                           unsigned characterSize, const Color&) override;
    void release(const GraphicsId&) override;
    void renderAll() override;
    void setPosition(const GraphicsId&, const utils::Vector2f& position) override;
    boost::optional<utils::Vector2f> getPosition(const GraphicsId&) override;
    void setTexture(const GraphicsId&, const TexturePath&, const utils::Vector2f& scale = {1, 1}) override;
    void setColor(const GraphicsId&, const Color&) override;
    void setRenderingSize(const utils::Vector2u& renderingSize) override;

private:
    void cleanUnusedShapes();
    std::vector<LayeredShape>::const_iterator findShapePosition(const GraphicsId&) const;
    std::vector<Text>::const_iterator findTextPosition(const GraphicsId&) const;

    std::unique_ptr<ContextRenderer> contextRenderer;
    std::unique_ptr<TextureStorage> textureStorage;
    std::unique_ptr<FontStorage> fontStorage;
    std::unique_ptr<GraphicsIdGenerator> idGenerator;
    std::vector<LayeredShape> layeredShapes;
    //std::vector<RectangleShape> layeredShapes;
    std::vector<Text> texts;
    std::unordered_set<GraphicsId, boost::hash<GraphicsId>> graphicsObjectsToRemove;
};
}
