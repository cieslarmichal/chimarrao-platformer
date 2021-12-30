#pragma once

#include "gmock/gmock.h"

#include "Input.h"

namespace input
{
class InputMock : public Input
{
public:
    MOCK_METHOD(bool, isKeyPressed, (InputKey), (const override));
    MOCK_METHOD(void, setKeyPressed, (InputKey), (override));
    MOCK_METHOD(bool, isKeyReleased, (InputKey), (const override));
    MOCK_METHOD(void, setReleasedKeys, (), (override));
    MOCK_METHOD(void, setMouseRelativePosition, (const utils::Vector2f&), (override));
    MOCK_METHOD(void, setMouseAbsolutePosition, (const utils::Vector2f&), (override));
    MOCK_METHOD(utils::Vector2f, getMouseRelativePosition, (), (const override));
    MOCK_METHOD(utils::Vector2f, getMouseAbsolutePosition, (), (const override));
    MOCK_METHOD(void, clearPressedKeys, (), (override));
};
}