#pragma once

#include <string>

#include "DeltaTime.h"
#include "Input.h"

namespace components::ui
{
class UIComponent
{
public:
    virtual ~UIComponent() = default;

    virtual void update(utils::DeltaTime, const input::Input&) = 0;
    virtual std::string getName() const = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
};
}