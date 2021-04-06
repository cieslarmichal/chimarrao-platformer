#pragma once

#include "gmock/gmock.h"

#include "RendererPool.h"

namespace graphics
{
class RendererPoolMock : public RendererPool
{
public:
    MOCK_METHOD(GraphicsId, acquire,
                (const utils::Vector2f& size, const utils::Vector2f& position, const Color&,
                 VisibilityLayer));
    MOCK_METHOD(GraphicsId, acquire,
                (const utils::Vector2f& size, const utils::Vector2f& position, const TexturePath&,
                 VisibilityLayer));
    MOCK_METHOD(GraphicsId, acquireText,
                (const utils::Vector2f& position, const std::string& text, const FontPath&,
                 unsigned characterSize, VisibilityLayer, const Color&));
    MOCK_METHOD(void, release, (const GraphicsId&));
    MOCK_METHOD(void, renderAll, ());
    MOCK_METHOD(void, setPosition, (const GraphicsId&, const utils::Vector2f&));
    MOCK_METHOD(boost::optional<utils::Vector2f>, getPosition, (const GraphicsId&));
    MOCK_METHOD(void, setTexture, (const GraphicsId&, const TextureRect&, const utils::Vector2f&));
    MOCK_METHOD(void, setText, (const GraphicsId&, const std::string&));
    MOCK_METHOD(void, setVisibility, (const GraphicsId&, VisibilityLayer));
    MOCK_METHOD(void, setColor, (const GraphicsId&, const Color&));
    MOCK_METHOD(void, setOutline, (const GraphicsId&, float, const Color&));
    MOCK_METHOD(void, setRenderingSize, (const utils::Vector2u&));
    MOCK_METHOD(void, synchronizeRenderingSize, ());
};
}