#pragma once

#include <string>
#include <vector>

#include "DeltaTime.h"
#include "Input.h"
#include "UIComponentType.h"
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
    virtual void setColor(const std::string& componentName, graphics::Color) = 0;
    virtual void changeClickAction(const std::string& componentName, const std::vector<core::KeyAction>&) = 0;
    virtual void invokeClickAction(const std::string& componentName,
                                   input::InputKey keyAssignedToClickAction) = 0;
    virtual void setText(const std::string& componentName, const std::string& text) = 0;
    virtual void setChecked(const std::string& componentName, bool checked) = 0;
    virtual void activateComponent(const std::string& componentName) = 0;
    virtual void deactivateComponent(const std::string& componentName) = 0;
    virtual bool isComponentActive(const std::string& componentName) const = 0;
    virtual void freezeAllButtons() = 0;
};
}