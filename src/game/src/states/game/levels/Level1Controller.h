#pragma once

#include "CharacterFactory.h"
#include "ComponentOwnersManager.h"
#include "Level1DialoguesController.h"
#include "Level1MainCharacters.h"
#include "LevelController.h"
#include "ObstacleFactory.h"
#include "TileMap.h"
#include "Timer.h"

namespace game
{
class Level1WorldBuilder;
class StoryGameState;

class Level1Controller : public LevelController
{
    friend class Level1WorldBuilder;

public:
    Level1Controller(const std::shared_ptr<TileMap>&, std::unique_ptr<ComponentOwnersManager>,
                     const std::shared_ptr<CharacterFactory>&, const std::shared_ptr<ObstacleFactory>&,
                     const std::shared_ptr<components::core::SharedContext>&,
                     std::shared_ptr<utils::FileAccess>, StoryGameState*);

    SwitchToNextLevel update(const utils::DeltaTime& deltaTime, const input::Input& input) override;
    void activate() override;
    void deactivate() override;
    Level1MainCharacters getCharacters() const;

private:
    void campfireAction();
    void druidAction();
    void deadPlayerAction();

    std::unique_ptr<Level1WorldBuilder> worldBuilder;
    Level1MainCharacters mainCharacters;
    std::unique_ptr<ComponentOwnersManager> ownersManager;
    std::shared_ptr<utils::FileAccess> fileAccess;
    StoryGameState* storyGameState;
    std::unique_ptr<Level1DialoguesController> dialoguesController;
    std::once_flag playerWithRabbitDialogueStarted;
    std::unique_ptr<utils::Timer> startFirstDialogueTimer;
    std::unique_ptr<utils::Timer> sleepTimer;
    std::unique_ptr<utils::Timer> deadTimer;
    const float timeNeededToStartFirstDialogue;
    const float sleepTime;
    const float deadTime;
    bool playerSleeping;
    bool playerDead;
};
}
