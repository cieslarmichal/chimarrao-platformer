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
    MOCK_METHOD(void, setColor, (const std::string& componentName, graphics::Color), (override));
    MOCK_METHOD(void, changeClickAction,
                (const std::string& componentName, const std::vector<core::KeyAction>&), (override));
    MOCK_METHOD(void, invokeClickAction,
                (const std::string& componentName, input::InputKey keyAssignedToClickAction), (override));
    MOCK_METHOD(void, invokeMouseOutAction, (const std::string& componentName), (override));
    MOCK_METHOD(void, setText, (const std::string& componentName, const std::string& text), (override));
    MOCK_METHOD(boost::optional<std::string>, getText, (const std::string& componentName), (const override));
    MOCK_METHOD(void, setChecked, (const std::string& componentName, bool checked), (override));
    MOCK_METHOD(void, activateComponent, (const std::string& componentName), (override));
    MOCK_METHOD(void, deactivateComponent, (const std::string& componentName), (override));
    MOCK_METHOD(bool, isComponentActive, (const std::string& componentName), (const override));
    MOCK_METHOD(void, freezeAllButtons, (), (override));
};
}