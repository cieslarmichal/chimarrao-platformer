#pragma once

#include "gmock/gmock.h"
#include "ContextRenderer.h"

namespace graphics
{
class ContextRendererMock : public ContextRenderer
{
public:
    MOCK_METHOD(void, initialize, ());
    MOCK_METHOD(void, clear, (const sf::Color&));
    MOCK_METHOD(void, draw, (const RectangleShape&));
    MOCK_METHOD(void, setView, ());
    MOCK_METHOD(void, setViewSize, (const utils::Vector2u& windowsSize));
};
}

