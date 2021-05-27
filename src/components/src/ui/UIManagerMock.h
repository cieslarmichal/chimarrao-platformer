#pragma once

#include "gmock/gmock.h"

#include "UIManager.h"

namespace components::ui
{
class UIManagerMock : public UIManager
{
public:
    MOCK_METHOD(void, createUI, (std::unique_ptr<UIConfig>), (override));
    MOCK_METHOD(void, update, (utils::DeltaTime, const input::Input&), (override));
    MOCK_METHOD(void, activate, (), (override));
    MOCK_METHOD(void, deactivate, (), (override));
    MOCK_METHOD(void, setColor, (UIComponentType, const std::string& componentName, graphics::Color),
                (override));
    MOCK_METHOD(void, changeClickAction,
                (UIComponentType, const std::string& componentName, const std::vector<core::KeyAction>&),
                (override));
    MOCK_METHOD(void, setText,
                (UIComponentTypeWithText, const std::string& componentName, const std::string& text),
                (override));
    MOCK_METHOD(void, setChecked, (UIComponentTypeWithCheck, const std::string& componentName, bool checked),
                (override));
    MOCK_METHOD(void, activateComponent, (UIComponentType, const std::string& componentName), (override));
    MOCK_METHOD(void, deactivateComponent, (UIComponentType, const std::string& componentName), (override));
    MOCK_METHOD(bool, isComponentActive, (UIComponentType, const std::string& componentName),
                (const override));
};
}