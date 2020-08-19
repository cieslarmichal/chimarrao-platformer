#pragma once

#include "gmock/gmock.h"

#include "WindowProxy.h"

namespace graphics
{
class WindowProxyMock : public WindowProxy
{
public:
    MOCK_METHOD(void, create, (utils::Vector2u windowSize, std::string windowTitle), ());
    MOCK_METHOD(bool, isOpen, (), (const));
    MOCK_METHOD(void, display, (), ());
    MOCK_METHOD(void, close, (), ());
    MOCK_METHOD(bool, pollEvent, (sf::Event & event), ());
};
}