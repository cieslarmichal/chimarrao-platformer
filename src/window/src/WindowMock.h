#pragma once

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
    MOCK_METHOD(utils::Vector2f, getMousePosition, (), (const override));
    MOCK_METHOD(void, registerObserver, (WindowObserver*), (override));
    MOCK_METHOD(void, removeObserver, (WindowObserver*), (override));
    MOCK_METHOD(void, notifyObservers, (), (override));
};
}