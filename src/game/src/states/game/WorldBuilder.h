#pragma once

#include "ComponentOwner.h"
#include "TileMap.h"

namespace game
{
class WorldBuilder
{
public:
    virtual ~WorldBuilder() = default;

    virtual std::vector<std::shared_ptr<components::core::ComponentOwner>>
    buildWorldObjects(const std::shared_ptr<TileMap>&) = 0;
    virtual std::shared_ptr<components::core::ComponentOwner> getPlayer() const = 0;
};
}
