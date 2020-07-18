#pragma once

#include "gmock/gmock.h"

#include "InputManager.h"

namespace game
{
class InputManagerMock : public InputManager
{
public:
    MOCK_METHOD(void, readInput(), ());
};
}