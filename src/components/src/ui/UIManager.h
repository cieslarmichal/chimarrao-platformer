#pragma once

#include <string>
#include <vector>

#include "DeltaTime.h"
#include "Input.h"
#include "UIComponentTypes.h"
#include "UIConfig.h"
#include "core/KeyAction.h"

namespace components::ui
{
class UIManager
{
public:
    virtual ~UIManager() = default;

    virtual void createUI(std::unique_ptr<UIConfig>) = 0;
    virtual void update(utils::DeltaTime, const input::Input&) = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void setColor(UIComponentType, const std::string& componentName, graphics::Color) = 0;
    virtual void changeClickAction(UIComponentType, const std::string& componentName,
                                   const std::vector<core::KeyAction>&) = 0;
    virtual void invokeClickAction(UIComponentType, const std::string& componentName,
                                   input::InputKey keyAssignedToClickAction) = 0;
    virtual void setText(UIComponentTypeWithText, const std::string& componentName,
                         const std::string& text) = 0;
    virtual void setChecked(UIComponentTypeWithCheck, const std::string& componentName, bool checked) = 0;
    virtual void activateComponent(UIComponentType, const std::string& componentName) = 0;
    virtual void deactivateComponent(UIComponentType, const std::string& componentName) = 0;
    virtual bool isComponentActive(UIComponentType, const std::string& componentName) const = 0;
};
}