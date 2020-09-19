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

    virtual void awake() {}
    virtual void initialize() {}
    virtual void update(utils::DeltaTime) {}
    virtual void lateUpdate(utils::DeltaTime) {}
    virtual void draw() {}

protected:
    ComponentOwner* owner;
};
}