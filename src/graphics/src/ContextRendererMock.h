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
    MOCK_METHOD(void, draw, (const sf::Drawable&));
    MOCK_METHOD(void, setView, ());
    MOCK_METHOD(const utils::Vector2f&, getViewSize, ());
    MOCK_METHOD(void, setViewSize, (const utils::Vector2u& windowsSize));
    MOCK_METHOD(void, synchronizeViewSize, ());
    MOCK_METHOD(void, setCenter, (const utils::Vector2f&));
    MOCK_METHOD(const utils::Vector2f&, getCenter, (), (const));
};
}
