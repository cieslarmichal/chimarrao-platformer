#pragma once

#include "gmock/gmock.h"

#include "Window.h"

namespace graphics
{
class WindowMock : public Window
{
public:
    MOCK_METHOD(bool, isOpen, (), (const));
    MOCK_METHOD(void, display, (), ());
    MOCK_METHOD(void, update, (), ());
    MOCK_METHOD(void, close, (), ());
};
}