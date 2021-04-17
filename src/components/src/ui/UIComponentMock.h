#pragma once

#include "gmock/gmock.h"

#include "UIComponent.h"

namespace components::ui
{
class UIComponentMock : public UIComponent
{
public:
    MOCK_METHOD(void, update, (), (override));
    MOCK_METHOD(std::string, getName, (), (const override));
    MOCK_METHOD(void, activate, (), (override));
    MOCK_METHOD(void, deactivate, (), (override));
};
}