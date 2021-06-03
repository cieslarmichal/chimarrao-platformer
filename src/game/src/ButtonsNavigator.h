#pragma once

#include <string>

#include "DeltaTime.h"
#include "Input.h"

namespace game
{
struct ButtonsNavigationConfig;

class ButtonsNavigator
{
public:
    virtual ~ButtonsNavigator() = default;

    virtual void initialize() = 0;
    virtual void update(const utils::DeltaTime&, const input::Input&) = 0;
    virtual void activate() = 0;
    virtual void setFocusOnButton(const std::string& buttonName) = 0;
    virtual void loseFocus() = 0;
};
}
