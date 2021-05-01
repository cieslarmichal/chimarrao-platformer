#pragma once

#include "CollisionSystem.h"
#include "ComponentOwnersManager.h"
#include "State.h"
#include "Timer.h"
#include "core/ComponentOwner.h"
#include "ui/UIManager.h"

namespace game
{
class GameState : public State
{
public:
    explicit GameState(const std::shared_ptr<window::Window>&, const std::shared_ptr<graphics::RendererPool>&,
                       std::shared_ptr<utils::FileAccess>, States&,
                       std::unique_ptr<components::ui::UIManager>, std::unique_ptr<ComponentOwnersManager>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    void pause();

    bool paused;
    utils::Timer timer;
    const float timeAfterStateCouldBePaused;
    std::unique_ptr<components::ui::UIManager> uiManager;
    std::unique_ptr<ComponentOwnersManager> componentOwnersManager;
};
}