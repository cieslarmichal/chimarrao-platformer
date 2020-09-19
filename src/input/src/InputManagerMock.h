#pragma once

#include "gmock/gmock.h"

#include "InputManager.h"

namespace input
{
class InputManagerMock : public InputManager
{
public:
    MOCK_METHOD(void, readInput, (), (override));
    MOCK_METHOD(void, registerObserver, (InputObserver*), (override));
    MOCK_METHOD(void, removeObserver, (InputObserver*), (override));
    MOCK_METHOD(void, notifyObservers, (const InputStatus&), (override));
};
}