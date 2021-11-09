#include "Component.h"

#include "ComponentOwner.h"

namespace components::core
{
Component::Component(ComponentOwner* ownerInit) : owner{ownerInit} {}

void Component::loadDependentComponents() {}

void Component::update(utils::DeltaTime, const input::Input&) {}

void Component::lateUpdate(utils::DeltaTime, const input::Input&) {}

void Component::enable()
{
    enabled = true;
}

void Component::disable()
{
    enabled = false;
}

bool Component::isEnabled() const
{
    return enabled;
}

std::string Component::getOwnerName() const
{
    return owner->getName();
}

unsigned int Component::getOwnerId() const
{
    return owner->getId();
}

bool Component::shouldBeRemoved() const
{
    return owner->shouldBeRemoved();
}

ComponentOwner& Component::getOwner() const
{
    return *owner;
}

}