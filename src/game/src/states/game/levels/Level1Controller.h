#pragma once

#include "CharacterFactory.h"
#include "ComponentOwnersManager.h"
#include "Level1MainCharacters.h"
#include "LevelController.h"
#include "ObstacleFactory.h"
#include "TileMap.h"
#include "Timer.h"

namespace game
{
class Level1WorldBuilder;

class Level1Controller : public LevelController
{
    friend class Level1WorldBuilder;

public:
    Level1Controller(const std::shared_ptr<TileMap>&, std::unique_ptr<ComponentOwnersManager>,
                     const std::shared_ptr<CharacterFactory>&, const std::shared_ptr<ObstacleFactory>&,
                     const std::shared_ptr<components::core::SharedContext>&, std::shared_ptr<utils::FileAccess>);

    SwitchToNextLevel update(const utils::DeltaTime& deltaTime, const input::Input& input) override;
    void activate() override;
    void deactivate() override;
    Level1MainCharacters getCharacters() const;

private:
    std::unique_ptr<utils::Timer> timer;
    Level1MainCharacters mainCharacters;
    std::unique_ptr<ComponentOwnersManager> ownersManager;
    std::shared_ptr<utils::FileAccess> fileAccess;
    bool playerBlocked{false};
    bool playerUnblocked{false};
};
}
