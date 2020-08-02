#pragma once

#include "gmock/gmock.h"
#include "RendererPool.h"

namespace graphics
{
class RendererPoolMock : public RendererPool
{
public:
    MOCK_METHOD(void, initialize, ());
};
}
    virtual void initialize() = 0;
    virtual void clear(const sf::Color&) = 0;
    virtual void draw(const RectangleShape&) = 0;
    virtual void setView() = 0;
    virtual void setViewSize(const utils::Vector2u& windowsSize) = 0;

