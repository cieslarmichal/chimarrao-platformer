#pragma once

#include "ComponentOwnersManager.h"
#include "Level1MainCharacters.h"
#include "LevelController.h"
#include "TileMap.h"
#include "WorldBuilder.h"

namespace game
{
class Level1Controller : public LevelController
{
public:
    Level1Controller(const std::shared_ptr<TileMap>&, std::unique_ptr<ComponentOwnersManager>,
                     std::shared_ptr<WorldBuilder>);

    SwitchToNextLevel update(const utils::DeltaTime& deltaTime, const input::Input& input) override;
    void activate() override;
    void deactivate() override;

private:
    Level1MainCharacters mainCharacters;
    std::unique_ptr<ComponentOwnersManager> ownersManager;
    std::shared_ptr<WorldBuilder> worldBuilder;
};
}
