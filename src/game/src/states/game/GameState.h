#pragma once

#include "CharacterFactory.h"
#include "CollisionSystem.h"
#include "ComponentOwner.h"
#include "ComponentOwnersManager.h"
#include "DefaultWorldBuilder.h"
#include "HeadsUpDisplay.h"
#include "ItemFactory.h"
#include "MusicManager.h"
#include "ObstacleFactory.h"
#include "RayCast.h"
#include "State.h"
#include "Timer.h"
#include "UIManager.h"
#include "TileMap.h"

namespace game
{
class GameState : public State
{
public:
    GameState(const std::shared_ptr<window::Window>&, const std::shared_ptr<graphics::RendererPool>&,
              std::shared_ptr<utils::FileAccess>, States&, std::shared_ptr<components::ui::UIManager>,
              std::unique_ptr<ComponentOwnersManager>, std::shared_ptr<TileMap>,
              const std::shared_ptr<components::core::SharedContext>&, std::shared_ptr<audio::MusicManager>,
              std::unique_ptr<WorldBuilder>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    void pause();

    bool paused;
    std::unique_ptr<utils::Timer> timer;
    const float timeAfterStateCouldBePaused;
    std::shared_ptr<components::ui::UIManager> uiManager;
    std::unique_ptr<ComponentOwnersManager> componentOwnersManager;
    std::shared_ptr<TileMap> tileMap;
    std::unique_ptr<HeadsUpDisplay> hud;
    const std::shared_ptr<components::core::SharedContext>& sharedContext;
    std::shared_ptr<audio::MusicManager> musicManager;
    audio::MusicId musicId;
    std::unique_ptr<WorldBuilder> worldBuilder;
};
}