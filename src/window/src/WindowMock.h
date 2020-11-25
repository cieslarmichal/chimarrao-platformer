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
    MOCK_METHOD(void, setDisplayMode, (DisplayMode), (override));
    MOCK_METHOD(void, setVerticalSync, (bool enabled), (override));
    MOCK_METHOD(void, setFramerateLimit, (unsigned int frameLimit), (override));
    MOCK_METHOD(void, setResolution, (const Resolution&), (override));
};
}