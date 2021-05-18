#include "DefaultComponentOwnersManager.h"

#include "core/GraphicsComponent.h"

namespace game
{

DefaultComponentOwnersManager::DefaultComponentOwnersManager(
    std::unique_ptr<physics::CollisionSystem> collisionSystemInit)
    : collisionSystem{std::move(collisionSystemInit)}
{
}

void DefaultComponentOwnersManager::add(std::shared_ptr<components::core::ComponentOwner> owner)
{
    newComponentOwners.push_back(owner);
}

void DefaultComponentOwnersManager::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    for (auto& componentOwner : componentOwners)
    {
        componentOwner->update(deltaTime, input);
    }

    collisionSystem->update();

    for (auto& componentOwner : componentOwners)
    {
        componentOwner->lateUpdate(deltaTime, input);
    }
}

void DefaultComponentOwnersManager::processNewObjects()
{
    if (not newComponentOwners.empty())
    {
        for (const auto& newComponentOwner : newComponentOwners)
        {
            newComponentOwner->loadDependentComponents();
        }

        componentOwners.insert(componentOwners.end(), newComponentOwners.begin(), newComponentOwners.end());
        collisionSystem->add(newComponentOwners);
        newComponentOwners.clear();
    }
}

void DefaultComponentOwnersManager::processRemovals()
{
    componentOwners.erase(std::remove_if(componentOwners.begin(), componentOwners.end(),
                                         [](const auto& componentOwner)
                                         { return componentOwner->shouldBeRemoved(); }),
                          componentOwners.end());

    collisionSystem->processRemovals();
}

void DefaultComponentOwnersManager::activate()
{
    for (auto& componentOwner : componentOwners)
    {
        componentOwner->enable();
    }
}

void DefaultComponentOwnersManager::deactivate()
{
    for (auto& componentOwner : componentOwners)
    {
        componentOwner->disable();
        if (auto graphics = componentOwner->getComponent<components::core::GraphicsComponent>())
        {
            graphics->enable();
        }
    }
}
}