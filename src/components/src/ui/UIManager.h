#pragma once

#include <string>

#include "UIComponentTypes.h"

namespace components::ui
{
class UIManager
{
public:
    virtual ~UIManager() = default;

    virtual void setColor(UIComponentType, const std::string& componentName, graphics::Color) = 0;
    virtual void setText(UIComponentTypeWithLabel, const std::string& componentName,
                         const std::string& text) = 0;
    virtual void setChecked(UIComponentTypeWithCheck, const std::string& componentName, bool checked) = 0;
};
}