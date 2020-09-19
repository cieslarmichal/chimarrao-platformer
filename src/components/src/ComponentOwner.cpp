#include "ComponentOwner.h"

namespace components
{
ComponentOwner::ComponentOwner(const utils::Vector2f& position)
{
    transform = addComponent<TransformComponent>(position);
}

void ComponentOwner::awake()
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->awake();
    }
}

void ComponentOwner::initialize()
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->initialize();
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

void ComponentOwner::draw()
{
    for (int i = components.size() - 1; i >= 0; i--)
    {
        components[i]->draw();
    }
}
}