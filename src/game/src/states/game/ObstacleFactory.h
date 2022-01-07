#pragma once

#include "AnimatorFactory.h"
#include "ComponentOwner.h"
#include "RendererPool.h"

namespace game
{
class ObstacleFactory
{
public:
    ObstacleFactory(const std::shared_ptr<components::core::SharedContext>&);

    std::shared_ptr<components::core::ComponentOwner> createBrick(const utils::Vector2f& position);
    std::shared_ptr<components::core::ComponentOwner> createGrass(const utils::Vector2f& position);
    std::shared_ptr<components::core::ComponentOwner> createTree(const utils::Vector2f& position);
    std::shared_ptr<components::core::ComponentOwner>
    createBush(const utils::Vector2f& position,
               const std::shared_ptr<components::core::ComponentOwner>& player);
    std::shared_ptr<components::core::ComponentOwner>
    createCampfire(const utils::Vector2f& position,
                   const std::shared_ptr<components::core::ComponentOwner>& player,
                   std::function<void(void)> action);
    std::shared_ptr<components::core::ComponentOwner> createChest(const utils::Vector2f& position);

private:
    const std::shared_ptr<components::core::SharedContext>& sharedContext;
    std::unique_ptr<animations::AnimatorFactory> animatorFactory;
};
}
