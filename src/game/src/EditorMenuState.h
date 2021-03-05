#pragma once

#include "InputObserver.h"
#include "State.h"
#include "Timer.h"
#include "core/ComponentOwner.h"

namespace game
{
class EditorMenuState : public State, public input::InputObserver
{
public:
    explicit EditorMenuState(const std::shared_ptr<window::Window>&,
                             const std::shared_ptr<input::InputManager>&,
                             const std::shared_ptr<graphics::RendererPool>&,
                             std::stack<std::unique_ptr<State>>&);
    ~EditorMenuState();

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;

private:
    void hideGraphics();
    void unfreezeButtons();
    void backToEditor();
    void backToMenu();
    void createEditorTitle();
    void createBackground();
    void createBackToEditorButton();
    void createLoadMapButton();
    void createNewMapButton();
    void createSaveMapButton();
    void createMenuButton();
    void addButton(const utils::Vector2f& position, const std::string& text,
                   const utils::Vector2f& textOffset, std::function<void(void)> clickAction);

    const input::InputStatus* inputStatus;
    utils::Timer possibleLeaveFromStateTimer;
    const float timeAfterLeaveStateIsPossible;
    bool shouldBackToEditor, shouldBackToMenu;
    std::unique_ptr<components::core::ComponentOwner> title;
    std::unique_ptr<components::core::ComponentOwner> background;
    std::vector<std::unique_ptr<components::core::ComponentOwner>> buttons;
    bool buttonsActionsFrozen = true;
    utils::Timer freezeClickableButtonsTimer;
    const float timeAfterButtonsCanBeClicked;
};
}