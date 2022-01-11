#include "DefaultComponentOwnersManager.h"

#include "GraphicsComponent.h"

namespace components::core
{

DefaultComponentOwnersManager::DefaultComponentOwnersManager(
    std::unique_ptr<physics::CollisionSystem> collisionSystemInit)
    : collisionSystem{std::move(collisionSystemInit)}
{
}

void DefaultComponentOwnersManager::add(std::shared_ptr<ComponentOwner> owner)
{
    newComponentOwners.push_back(owner);
}

void DefaultComponentOwnersManager::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    processNewObjects();

    std::cerr << componentOwners.size()<< std::endl;
    for (auto& componentOwner : componentOwners)
    {
        componentOwner->update(deltaTime, input);
    }

    collisionSystem->update();

    for (auto& componentOwner : componentOwners)
    {
        componentOwner->lateUpdate(deltaTime, input);
    }

    processRemovals();
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
        collisionSystem->add(componentOwners);
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
        for (auto& graphics : componentOwner->allGraphics)
        {
            graphics->enable();
        }
    }
}
}