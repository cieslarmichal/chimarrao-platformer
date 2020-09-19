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

protected:
    ComponentOwner* owner;
};
}