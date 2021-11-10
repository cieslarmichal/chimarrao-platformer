#pragma once

#include <memory>

#include "AnimatorFactory.h"
#include "Quadtree.h"
#include "RayCast.h"
#include "RendererPool.h"
#include "core/ComponentOwner.h"
#include "editor/TileMap.h"

namespace game
{
class CharacterFactory
{
public:
    CharacterFactory(const std::shared_ptr<components::core::SharedContext>&);

    std::shared_ptr<components::core::ComponentOwner> createYerba(const utils::Vector2f& position);
    std::shared_ptr<components::core::ComponentOwner> createApple(const utils::Vector2f& position);
    std::shared_ptr<components::core::ComponentOwner> createMeat(const utils::Vector2f& position);

private:
    const std::shared_ptr<components::core::SharedContext>& sharedContext;
};
}