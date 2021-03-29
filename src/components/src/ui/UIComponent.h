#pragma once

#include <string>

#include "DeltaTime.h"

namespace components::ui
{
class UIComponent
{
public:
    ~UIComponent() = default;

    virtual void update(utils::DeltaTime) = 0;
    virtual std::string getName() const = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
};
}