#pragma once

#include "State.h"
#include "Timer.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"

namespace game
{
class PauseState : public State
{
    friend class PauseStateUIConfigBuilder;

public:
    explicit PauseState(const std::shared_ptr<window::Window>&,
                        const std::shared_ptr<graphics::RendererPool>&, std::shared_ptr<utils::FileAccess>,
                        States&, std::unique_ptr<components::ui::UIManager>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    utils::Timer timer;
    const float timeAfterLeaveStateIsPossible;
    bool shouldBackToGame;
    bool shouldBackToMenu;
    std::unique_ptr<components::ui::UIManager> uiManager;
};
}