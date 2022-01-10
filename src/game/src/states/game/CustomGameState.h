#pragma once

#include <queue>

#include "CharacterFactory.h"
#include "CollisionSystem.h"
#include "ComponentOwner.h"
#include "ComponentOwnersManager.h"
#include "HeadsUpDisplay.h"
#include "ItemFactory.h"
#include "Level1WorldBuilder.h"
#include "LevelController.h"
#include "MusicManager.h"
#include "ObstacleFactory.h"
#include "PhysicsFactory.h"
#include "RayCast.h"
#include "State.h"
#include "TileMap.h"
#include "Timer.h"
#include "UIManager.h"

namespace game
{
class CustomGameState : public State
{
public:
    CustomGameState(const std::shared_ptr<window::Window>&, const std::shared_ptr<graphics::RendererPool>&,
                    std::shared_ptr<utils::FileAccess>, States&, std::shared_ptr<components::ui::UIManager>,
                    std::shared_ptr<TileMap>, const std::shared_ptr<components::core::SharedContext>&,
                    std::shared_ptr<audio::MusicManager>, const std::shared_ptr<CharacterFactory>&,
                    const std::shared_ptr<ObstacleFactory>&, std::unique_ptr<physics::PhysicsFactory>);

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
    std::shared_ptr<TileMap> tileMap;
    std::unique_ptr<HeadsUpDisplay> hud;
    const std::shared_ptr<components::core::SharedContext>& sharedContext;
    std::shared_ptr<audio::MusicManager> musicManager;
    audio::MusicId musicId;
    std::unique_ptr<components::core::ComponentOwnersManager> ownersManager;
};
}