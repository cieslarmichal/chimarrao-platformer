#pragma once

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
                       States&, std::unique_ptr<components::ui::UIManager>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;
    void pause();

private:
    bool paused;
    utils::Timer timer;
    const float timeAfterStateCouldBePaused;
    std::shared_ptr<components::core::ComponentOwner> player;
    std::unique_ptr<components::ui::UIManager> uiManager;
};
}