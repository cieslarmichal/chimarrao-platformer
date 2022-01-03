#pragma once

#include "LevelController.h"
#include "TileMap.h"
#include "Level1MainCharacters.h"

namespace game
{
class Level1Controller : public LevelController
{
public:
    Level1Controller(const std::shared_ptr<TileMap>&, std::shared_ptr<Level1MainCharacters>);

    SwitchToNextLevel update() override;

private:
    std::shared_ptr<Level1MainCharacters> mainCharacters;
};
}

