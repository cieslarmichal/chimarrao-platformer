#pragma once

#include "CharacterFactory.h"
#include "ComponentOwnersManager.h"
#include "Level1DialoguesController.h"
#include "Level1MainCharacters.h"
#include "LevelController.h"
#include "ObstacleFactory.h"
#include "TileMap.h"
#include "Timer.h"
#include "UIManager.h"

namespace game
{
class Level1WorldBuilder;
class StoryGameState;

class Level1Controller : public LevelController
{
    friend class Level1WorldBuilder;

public:
    Level1Controller(const std::shared_ptr<TileMap>&,
                     std::shared_ptr<components::core::ComponentOwnersManager>,
                     const std::shared_ptr<CharacterFactory>&, const std::shared_ptr<ObstacleFactory>&,
                     const std::shared_ptr<ItemFactory>&,
                     const std::shared_ptr<components::core::SharedContext>&,
                     std::shared_ptr<utils::FileAccess>, std::shared_ptr<components::ui::UIManager>,
                     StoryGameState*);

    SwitchToNextLevel update(const utils::DeltaTime& deltaTime, const input::Input& input) override;
    void activate() override;
    void deactivate() override;
    Level1MainCharacters getCharacters() const;

private:
    void firstCampfireAction();
    void lastCampfireAction();
    void druidFirstAction();
    void druidSecondAction();
    void deadPlayerAction();

    std::shared_ptr<components::core::ComponentOwnersManager> ownersManager;
    std::unique_ptr<Level1WorldBuilder> worldBuilder;
    Level1MainCharacters mainCharacters;
    std::shared_ptr<utils::FileAccess> fileAccess;
    std::shared_ptr<components::ui::UIManager> uiManager;
    StoryGameState* storyGameState;
    std::unique_ptr<Level1DialoguesController> dialoguesController;
    std::once_flag playerWithRabbitDialogueStarted;
    std::once_flag backgroundChanged;
    std::unique_ptr<utils::Timer> startFirstDialogueTimer;
    std::unique_ptr<utils::Timer> sleepTimer;
    std::unique_ptr<utils::Timer> deadTimer;
    const float timeNeededToStartFirstDialogue;
    const float sleepTime;
    const float deadTime;
    bool playerSleeping;
    bool playerDead;
    bool playerSleepingNextToLastCampfire;
    bool levelFinished;
    bool playerTalkedToDruid;
};
}
