#pragma once

#include <utility>

#include "ComponentOwner.h"

namespace game
{
struct Level1MainCharacters
{
    std::shared_ptr<components::core::ComponentOwner> player;
    std::shared_ptr<components::core::ComponentOwner> rabbit;
    std::shared_ptr<components::core::ComponentOwner> npc;
};
}