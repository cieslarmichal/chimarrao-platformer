#pragma once

#include "gmock/gmock.h"

#include "States.h"

namespace game
{
class StatesMock : public States
{
public:
    MOCK_METHOD(StatesStatus, updateCurrentState, (const utils::DeltaTime&, const input::Input&),
                (override));
    MOCK_METHOD(void, deactivateCurrentState, (), (override));
    MOCK_METHOD(void, addNextState, (StateType), (override));
    MOCK_METHOD(void, render, (), (override));
};
}