#pragma once

#include <utility>

#include "ComponentOwner.h"

namespace game
{
struct Level1MainCharacters
{
    Level1MainCharacters(std::shared_ptr<components::core::ComponentOwner> player,
                          std::shared_ptr<components::core::ComponentOwner> rabbit,
                          std::shared_ptr<components::core::ComponentOwner> npc)
        : player{std::move(player)}, rabbit{std::move(rabbit)}, npc{std::move(npc)}
    {
    }

    std::shared_ptr<components::core::ComponentOwner> player;
    std::shared_ptr<components::core::ComponentOwner> rabbit;
    std::shared_ptr<components::core::ComponentOwner> npc;
};
}