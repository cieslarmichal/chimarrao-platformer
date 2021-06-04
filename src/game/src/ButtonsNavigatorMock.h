#pragma once

#include "gmock/gmock.h"

#include "ButtonsNavigator.h"
#include "NextState.h"

namespace game
{
class ButtonsNavigatorMock : public ButtonsNavigator
{
public:
    MOCK_METHOD(void, initialize, (), (override));
    MOCK_METHOD(NextState, update, (const utils::DeltaTime&, const input::Input&), (override));
    MOCK_METHOD(void, activate, (), (override));
    MOCK_METHOD(void, setFocusOnButton, (const std::string& buttonName), (override));
    MOCK_METHOD(void, loseFocus, (), (override));
};
}