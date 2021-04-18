#pragma once

#include "Game.h"

namespace game
{
class GameFactory
{
public:
    static std::unique_ptr<Game> createGame();
};
}