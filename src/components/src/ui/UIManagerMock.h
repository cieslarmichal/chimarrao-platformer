#pragma once

#include "gmock/gmock.h"

#include "UIManager.h"

namespace input
{
class UIManagerMock : public UIManager
{
public:
    MOCK_METHOD(void, createUI, (std::unique_ptr<UIConfig>), (override));
    MOCK_METHOD(void, activate, (), (override));
    MOCK_METHOD(void, deactivate, (), (override));
    MOCK_METHOD(void, setColor, (UIComponentType, const std::string& componentName, graphics::Color),
                (override));
    MOCK_METHOD(void, setText,
                (UIComponentTypeWithLabel, const std::string& componentName, const std::string& text),
                (override));
    MOCK_METHOD(void, setText, (UIComponentTypeWithCheck, const std::string& componentName, bool checked),
                (override));
};
}