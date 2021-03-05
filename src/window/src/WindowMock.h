#pragma once

#include "SFML/Graphics/View.hpp"
#include "SFML/Window/Event.hpp"
#include "gmock/gmock.h"

#include "Window.h"

namespace window
{
class WindowMock : public Window
{
public:
    MOCK_METHOD(bool, isOpen, (), (const override));
    MOCK_METHOD(void, display, (), (override));
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(void, close, (), (override));
    MOCK_METHOD(void, setView, (const sf::View&), (override));
    MOCK_METHOD(bool, pollEvent, (sf::Event&), (const override));
    MOCK_METHOD(utils::Vector2f, getMousePosition, (), (const override));
    MOCK_METHOD(void, registerObserver, (WindowObserver*), (override));
    MOCK_METHOD(void, removeObserver, (WindowObserver*), (override));
    MOCK_METHOD(void, notifyObservers, (), (override));
    MOCK_METHOD(WindowSettings, getWindowSettings, (), (const override));
    MOCK_METHOD(bool, setDisplayMode, (DisplayMode), (override));
    MOCK_METHOD(bool, setVerticalSync, (bool enabled), (override));
    MOCK_METHOD(bool, setFramerateLimit, (unsigned int frameLimit), (override));
    MOCK_METHOD(bool, setResolution, (const Resolution&), (override));
    MOCK_METHOD(std::vector<Resolution>, getSupportedResolutions, (), (const override));
    MOCK_METHOD(std::vector<unsigned int>, getSupportedFrameLimits, (), (const override));
};
}