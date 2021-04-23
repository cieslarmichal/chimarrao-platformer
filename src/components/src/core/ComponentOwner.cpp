#include "ComponentOwner.h"

#include <algorithm>

namespace components::core
{
ComponentOwner::ComponentOwner(const utils::Vector2f& position, const std::string& uniqueNameInit)
    : uniqueName{uniqueNameInit}
{
    transform = addComponent<TransformComponent>(position);
}

void ComponentOwner::loadDependentComponents()
{
    for (int i = static_cast<int>(components.size() - 1); i >= 0; i--)
    {
        components[i]->loadDependentComponents();
    }
}

void ComponentOwner::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    for (int i = static_cast<int>(components.size() - 1); i >= 0; i--)
    {
        components[i]->update(deltaTime, input);
    }
}

void ComponentOwner::lateUpdate(utils::DeltaTime deltaTime)
{
    for (int i = static_cast<int>(components.size() - 1); i >= 0; i--)
    {
        components[i]->lateUpdate(deltaTime);
    }
}

void ComponentOwner::enable()
{
    for (auto& component : components)
    {
        component->enable();
    }
}

void ComponentOwner::disable()
{
    for (auto& component : components)
    {
        component->disable();
    }
}

std::string ComponentOwner::getName() const
{
    return uniqueName.getName();
}

bool ComponentOwner::areComponentEnabled() const
{
    return std::all_of(components.begin(), components.end(),
                       [](const auto& component) { return component->isEnabled(); });
}

}