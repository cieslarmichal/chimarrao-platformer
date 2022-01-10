#pragma once

#include <memory>

#include "AnimatorFactory.h"
#include "ComponentOwner.h"
#include "ComponentOwnersManager.h"
#include "Quadtree.h"
#include "RayCast.h"
#include "RendererPool.h"
#include "TileMap.h"

namespace game
{
class CharacterFactory
{
public:
    CharacterFactory(const std::shared_ptr<components::core::SharedContext>&, std::shared_ptr<TileMap>,
                     std::shared_ptr<physics::RayCast>, std::shared_ptr<physics::Quadtree>,
                     std::shared_ptr<components::core::ComponentOwnersManager>);

    std::shared_ptr<components::core::ComponentOwner> createPlayer(const utils::Vector2f& position,
                                                                   std::function<void(void)> deadAction);
    std::shared_ptr<components::core::ComponentOwner>
    createRabbitFollower(const std::shared_ptr<components::core::ComponentOwner>& player,
                         const utils::Vector2f& position);
    std::shared_ptr<components::core::ComponentOwner>
    createDruidNpc(const std::shared_ptr<components::core::ComponentOwner>& player,
                   const utils::Vector2f& position, std::function<void(void)> interaction);
    std::shared_ptr<components::core::ComponentOwner>
    createBanditEnemy(const std::shared_ptr<components::core::ComponentOwner>& player,
                      const utils::Vector2f& position);

private:
    const std::shared_ptr<components::core::SharedContext>& sharedContext;
    std::shared_ptr<TileMap> tileMap;
    std::shared_ptr<physics::RayCast> rayCast;
    std::shared_ptr<physics::Quadtree> quadtree;
    std::unique_ptr<animations::AnimatorFactory> animatorFactory;
    std::shared_ptr<components::core::ComponentOwnersManager> ownersManager;
};
}