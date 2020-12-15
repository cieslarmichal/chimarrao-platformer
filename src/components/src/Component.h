#pragma once

#include <memory>

#include "DeltaTime.h"

namespace components
{
class ComponentOwner;

class Component
{
public:
    Component(ComponentOwner* ownerInit) : owner{ownerInit} {}

    virtual void loadDependentComponents() {}
    virtual void start() {}
    virtual void update(utils::DeltaTime) {}
    virtual void lateUpdate(utils::DeltaTime) {}
    virtual void enable()
    {
        enabled = true;
    };
    virtual void disable()
    {
        enabled = false;
    };
    bool isEnabled() const
    {
        return enabled;
    }

protected:
    ComponentOwner* owner;
    bool enabled{true};
};
}