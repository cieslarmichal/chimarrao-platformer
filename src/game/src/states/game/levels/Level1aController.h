#pragma once

#include "LevelController.h"
#include "TileMap.h"
#include "Level1aMainCharacters.h"

namespace game
{
class Level1aController : public LevelController
{
public:
    Level1aController(const std::shared_ptr<TileMap>&, std::shared_ptr<Level1aMainCharacters>);

    SwitchToNextLevel update() override;

private:
    std::shared_ptr<Level1aMainCharacters> mainCharacters;
};
}

