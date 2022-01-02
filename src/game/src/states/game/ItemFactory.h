#pragma once

#include <memory>

#include "ComponentOwner.h"
#include "SharedContext.h"

namespace game
{
class ItemFactory
{
public:
    explicit ItemFactory(const std::shared_ptr<components::core::SharedContext>&);

    std::shared_ptr<components::core::ComponentOwner> createYerba(const utils::Vector2f& position);
    std::shared_ptr<components::core::ComponentOwner> createApple(const utils::Vector2f& position);
    std::shared_ptr<components::core::ComponentOwner> createMeat(const utils::Vector2f& position);

private:
    const std::shared_ptr<components::core::SharedContext>& sharedContext;
};
}