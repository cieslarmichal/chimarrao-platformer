#include "ComponentOwner.h"

namespace components::core
{
ComponentOwner::ComponentOwner(const utils::Vector2f& position, const std::string& uniqueNameInit)
    : uniqueName{uniqueNameInit}
{
    transform = addComponent<TransformComponent>(position);
}

void ComponentOwner::loadDependentComponents()
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->loadDependentComponents();
    }
}

void ComponentOwner::update(utils::DeltaTime deltaTime)
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->update(deltaTime);
    }
}

void ComponentOwner::lateUpdate(utils::DeltaTime deltaTime)
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->lateUpdate(deltaTime);
    }
}

void ComponentOwner::enable()
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->enable();
    }
}

void ComponentOwner::disable()
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->disable();
    }
}

std::string ComponentOwner::getName() const
{
    return uniqueName.getName();
}

}