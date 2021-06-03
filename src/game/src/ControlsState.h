#pragma once

#include "State.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"

namespace game
{
class ControlsStateUIConfigBuilder;

class ControlsState : public State
{
    friend class ControlsStateUIConfigBuilder;

public:
    explicit ControlsState(const std::shared_ptr<window::Window>&,
                           const std::shared_ptr<graphics::RendererPool>&, std::shared_ptr<utils::FileAccess>,
                           States&, std::shared_ptr<components::ui::UIManager>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    bool shouldBackToMenu;
    std::shared_ptr<components::ui::UIManager> uiManager;
};
}