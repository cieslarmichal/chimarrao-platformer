#pragma once

#include <memory>

#include "DeltaTime.h"
#include "Input.h"

namespace components::core
{
class ComponentOwner;

class Component
{
public:
    explicit Component(ComponentOwner*);
    virtual ~Component() = default;

    virtual void loadDependentComponents();
    virtual void update(utils::DeltaTime, const input::Input&);
    virtual void lateUpdate(utils::DeltaTime, const input::Input& input);
    virtual void enable();
    virtual void disable();
    virtual bool isEnabled() const;
    std::string getOwnerName() const;
    unsigned int getOwnerId() const;
    bool shouldBeRemoved() const;
    ComponentOwner& getOwner() const;

protected:
    ComponentOwner* owner;
    bool enabled{true};
};
}