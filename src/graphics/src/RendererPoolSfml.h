#pragma once

#include <boost/functional/hash.hpp>
#include <memory>
#include <unordered_set>
#include <vector>

#include "ContextRenderer.h"
#include "GraphicsIdGenerator.h"
#include "RectangleShape.h"
#include "RendererPool.h"
#include "TextureStorage.h"

namespace graphics
{
class RendererPoolSfml : public RendererPool
{
public:
    RendererPoolSfml(std::unique_ptr<ContextRenderer>, std::unique_ptr<TextureStorage>);

    GraphicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position, const Color&) override;
    GraphicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position,
                       const TexturePath&) override;
    void release(const GraphicsId&) override;
    void renderAll() override;
    void setPosition(const GraphicsId&, const utils::Vector2f& position) override;
    // TODO: add optional to getPosition
    boost::optional<utils::Vector2f> getPosition(const GraphicsId&) override;
    void setTexture(const GraphicsId&, const TexturePath&, const utils::Vector2f& scale = {1, 1}) override;
    void setRenderingSize(const utils::Vector2u& renderingSize) override;

private:
    void cleanUnusedShapes();
    std::vector<RectangleShape>::const_iterator findShapePosition(const GraphicsId&) const;

    std::unique_ptr<ContextRenderer> contextRenderer;
    std::unique_ptr<TextureStorage> textureStorage;
    std::unique_ptr<GraphicsIdGenerator> idGenerator;
    std::vector<RectangleShape> shapes;
    std::unordered_set<GraphicsId, boost::hash<GraphicsId>> shapesToRemove;
};
}
