#pragma once

#include "ComponentOwner.h"
#include "InputObserver.h"
#include "State.h"
#include "Timer.h"

namespace game
{
class SettingsState : public State, public input::InputObserver
{
public:
    explicit SettingsState(const std::shared_ptr<window::Window>&,
                           const std::shared_ptr<input::InputManager>&,
                           const std::shared_ptr<graphics::RendererPool>&,
                           std::stack<std::unique_ptr<State>>&);
    ~SettingsState();

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;

private:
    void synchronizeWindowSettings();
    void applyWindowSettingsChanges();
    void unfreezeButtons();
    void backToMenu();
    void createBackground();
    void createSettingsTitle();
    void createBackToMenuButton();
    void createApplyChangesButton();
    void createDisplayModeSection();
    void createResolutionSection();
    void createVsyncSection();
    void createFrameLimitSection();
    void addButton(const utils::Vector2f& position, const utils::Vector2f& size, const std::string& text,
                   unsigned int fontSize, const utils::Vector2f& textOffset,
                   std::function<void(void)> clickAction);
    void addText(const utils::Vector2f& position, const std::string& description, unsigned int fontSize);

    bool shouldBackToMenu;
    const input::InputStatus* inputStatus;
    std::unique_ptr<components::ComponentOwner> background;
    std::vector<std::unique_ptr<components::ComponentOwner>> texts;
    std::vector<std::unique_ptr<components::ComponentOwner>> buttons;
    window::WindowSettings selectedWindowsSettings;
    std::vector<window::Resolution> supportedResolutions;
    int selectedResolutionIndex = 0;
    std::vector<unsigned int> supportedFrameLimits;
    int selectedFrameLimitIndex = 0;
    bool buttonsActionsFrozen = true;
    utils::Timer freezeClickableButtonsTimer;
    const float timeAfterButtonsCanBeClicked;
};
}