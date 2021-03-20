#pragma once

#include "InputObserver.h"
#include "State.h"
#include "Timer.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"
#include "ui/DefaultUIManager.h"

namespace game
{
class SaveMapStateUIConfigBuilder;

class SaveMapState : public State, public input::InputObserver
{
    friend class SaveMapStateUIConfigBuilder;

public:
    explicit SaveMapState(const std::shared_ptr<window::Window>&, const std::shared_ptr<input::InputManager>&,
                          const std::shared_ptr<graphics::RendererPool>&,
                          std::stack<std::unique_ptr<State>>&, std::unique_ptr<components::ui::UIManager>);
    ~SaveMapState();

    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;

private:
    void saveMap();
    void backToEditorMenu();

    const input::InputStatus* inputStatus;
    utils::Timer possibleLeaveFromStateTimer;
    const float timeAfterLeaveStateIsPossible;
    bool shouldBackToEditorMenu;
    std::string currentMapName;
    std::unique_ptr<components::ui::UIManager> uiManager;
};
}