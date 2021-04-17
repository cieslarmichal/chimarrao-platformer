#pragma once

#include "gmock/gmock.h"

#include "InputManager.h"

namespace input
{
class InputManagerMock : public InputManager
{
public:
    MOCK_METHOD(const Input&, readInput, (), (override));
};
}