#pragma once

#include "InputObserver.h"
#include "State.h"
#include "Timer.h"
#include "core/ComponentOwner.h"
#include "editor/TileMap.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"

namespace game
{
class EditorMenuStateUIConfigBuilder;

class EditorMenuState : public State, public input::InputObserver
{
public:
    friend class EditorMenuStateUIConfigBuilder;

    explicit EditorMenuState(const std::shared_ptr<window::Window>&,
                             const std::shared_ptr<input::InputManager>&,
                             const std::shared_ptr<graphics::RendererPool>&,
                             const std::shared_ptr<utils::FileAccess>&,
                             std::stack<std::unique_ptr<State>>&, std::unique_ptr<components::ui::UIManager>,
                             TileMap&);
    ~EditorMenuState();

    NextState update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;

private:
    const input::InputStatus* inputStatus;
    utils::Timer possibleLeaveFromStateTimer;
    const float timeAfterLeaveStateIsPossible;
    bool shouldBackToEditor;
    bool shouldBackToMenu;
    std::unique_ptr<components::ui::UIManager> uiManager;
    TileMap& tileMap;
};
}