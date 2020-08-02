#pragma once

#include <memory>
#include <vector>
#include <boost/functional/hash.hpp>

#include "GraphicsIdGenerator.h"
#include "RectangleShape.h"
#include "RendererPool.h"
#include "TextureStorage.h"
#include "ContextRenderer.h"
#include <unordered_set>

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
    void setPosition(const GraphicsId&, const utils::Vector2f&) override;
    utils::Vector2f getPosition(const GraphicsId&) override;
    void setTexture(const GraphicsId&, const TexturePath&) override;
    void setRenderingSize(const utils::Vector2u&) override;

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
