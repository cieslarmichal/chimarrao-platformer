#pragma once

#include "State.h"
#include "Timer.h"
#include "UIConfig.h"
#include "UIManager.h"
#include "navigators/GridButtonsNavigator.h"

namespace game
{
class GameOverState : public State
{
    friend class GameOverStateUIConfigBuilder;

public:
    GameOverState(const std::shared_ptr<window::Window>&, const std::shared_ptr<graphics::RendererPool>&,
                  std::shared_ptr<utils::FileAccess>, States&, std::shared_ptr<components::ui::UIManager>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    void backToMenu();

    bool shouldBackToMenu;
    std::shared_ptr<components::ui::UIManager> uiManager;
};
}