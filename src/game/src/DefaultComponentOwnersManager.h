#pragma once

#include "CollisionSystem.h"
#include "ComponentOwnersManager.h"

namespace game
{
class DefaultComponentOwnersManager : public ComponentOwnersManager
{
public:
    explicit DefaultComponentOwnersManager(std::unique_ptr<physics::CollisionSystem>);

    void add(std::shared_ptr<components::core::ComponentOwner>) override;
    void update(const utils::DeltaTime&, const input::Input&) override;
    void processNewObjects() override;
    void processRemovals() override;
    void activate() override;
    void deactivate() override;

private:
    std::vector<std::shared_ptr<components::core::ComponentOwner>> componentOwners;
    std::vector<std::shared_ptr<components::core::ComponentOwner>> newComponentOwners;
    std::unique_ptr<physics::CollisionSystem> collisionSystem;
};
}
