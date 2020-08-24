#pragma once

#include "gmock/gmock.h"

#include "RendererPool.h"

namespace graphics
{
class RendererPoolMock : public RendererPool
{
public:
    MOCK_METHOD(GraphicsId, acquire,
                (const utils::Vector2f& size, const utils::Vector2f& position, const Color&));
    MOCK_METHOD(GraphicsId, acquire,
                (const utils::Vector2f& size, const utils::Vector2f& position, const TexturePath&));
    MOCK_METHOD(void, release, (const GraphicsId&));
    MOCK_METHOD(void, renderAll, ());
    MOCK_METHOD(void, setPosition, (const GraphicsId&, const utils::Vector2f&));
    MOCK_METHOD(utils::Vector2f, getPosition, (const GraphicsId&));
    MOCK_METHOD(void, setTexture, (const GraphicsId&, const TexturePath&, const utils::Vector2f&));
    MOCK_METHOD(void, setRenderingSize, (const utils::Vector2u&));
};
}