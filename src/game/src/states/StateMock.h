#pragma once

#include "gmock/gmock.h"

#include "State.h"

namespace game
{
class StateMock : public State
{
public:
    MOCK_METHOD(NextState, update, (const utils::DeltaTime&), (override));
    MOCK_METHOD(void, lateUpdate, (const utils::DeltaTime&), (override));
    MOCK_METHOD(void, render, (), (override));
    MOCK_METHOD(StateType, getType, (), (const override));
    MOCK_METHOD(void, activate, (), (override));
    MOCK_METHOD(void, deactivate, (), (override));
};
}