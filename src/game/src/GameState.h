#pragma once

#include "CollisionSystem.h"
#include "ComponentOwnersManager.h"
#include "HeadsUpDisplay.h"
#include "RayCast.h"
#include "State.h"
#include "Timer.h"
#include "core/ComponentOwner.h"
#include "editor/TileMap.h"
#include "ui/UIManager.h"

namespace game
{
class GameState : public State
{
public:
    explicit GameState(const std::shared_ptr<window::Window>&, const std::shared_ptr<graphics::RendererPool>&,
                       std::shared_ptr<utils::FileAccess>, States&,
                       std::shared_ptr<components::ui::UIManager>, std::unique_ptr<ComponentOwnersManager>,
                       std::shared_ptr<TileMap>, std::shared_ptr<physics::RayCast>);

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
    std::shared_ptr<physics::RayCast> rayCast;
};
}