#include "ComponentOwner.h"

#include <algorithm>

namespace components::core
{
ComponentOwner::ComponentOwner(const utils::Vector2f& position, const std::string& uniqueNameInit)
    : uniqueName{uniqueNameInit}
{
    transform = addComponent<TransformComponent>(position);
    id = addComponent<IdComponent>();
}

void ComponentOwner::loadDependentComponents()
{
    for (auto& component : components)
    {
        component->loadDependentComponents();
    }

    for (auto& graphics : allGraphics)
    {
        graphics->loadDependentComponents();
    }
}

void ComponentOwner::update(utils::DeltaTime deltaTime, const input::Input& input)
{
    for (int i = static_cast<int>(components.size() - 1); i >= 0; i--)
    {
        components[i]->update(deltaTime, input);
    }

    for (auto& graphics : allGraphics)
    {
        graphics->update(deltaTime, input);
    }
}

void ComponentOwner::lateUpdate(utils::DeltaTime deltaTime, const input::Input& input)
{
    for (int i = static_cast<int>(components.size() - 1); i >= 0; i--)
    {
        components[i]->lateUpdate(deltaTime, input);
    }

    for (auto& graphics : allGraphics)
    {
        graphics->lateUpdate(deltaTime, input);
    }
}

void ComponentOwner::enable()
{
    for (auto& component : components)
    {
        component->enable();
    }

    for (auto& graphics : allGraphics)
    {
        graphics->enable();
    }
}

void ComponentOwner::disable()
{
    for (auto& component : components)
    {
        component->disable();
    }

    for (auto& graphics : allGraphics)
    {
        graphics->disable();
    }
}

std::string ComponentOwner::getName() const
{
    return uniqueName.getName();
}

unsigned int ComponentOwner::getId() const
{
    return id->getId();
}

bool ComponentOwner::areComponentsEnabled() const
{
    return std::all_of(components.begin(), components.end(),
                       [](const auto& component) { return component->isEnabled(); });
}

void ComponentOwner::remove()
{
    toRemove = true;
}

bool ComponentOwner::shouldBeRemoved() const
{
    return toRemove;
}

}