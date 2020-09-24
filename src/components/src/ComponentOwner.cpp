#include "ComponentOwner.h"

namespace components
{
ComponentOwner::ComponentOwner(const utils::Vector2f& position)
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

void ComponentOwner::start()
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->start();
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

}