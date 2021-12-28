#pragma once

#include "GridButtonsNavigator.h"
#include "State.h"
#include "Timer.h"
#include "Window.h"
#include "ComponentOwner.h"
#include "UIConfig.h"
#include "UIManager.h"

namespace game
{
class MenuStateUIConfigBuilder;

class MenuState : public State
{
public:
    friend class MenuStateUIConfigBuilder;

    explicit MenuState(const std::shared_ptr<window::Window>&, const std::shared_ptr<graphics::RendererPool>&,
                       std::shared_ptr<utils::FileAccess>, States&,
                       std::shared_ptr<components::ui::UIManager>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;
    void handleWindowSizeChange(const utils::Vector2u& windowSize) override;

private:
    void runGame();
    void runMapEditor();
    void runControls();
    void runSettings();
    void exit();

    std::shared_ptr<components::ui::UIManager> uiManager;
    std::unique_ptr<GridButtonsNavigator> buttonsNavigator;
    bool shouldExit;
};
}