#pragma once

#include "InputObserver.h"
#include "State.h"
#include "Timer.h"
#include "core/ComponentOwner.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"

namespace game
{
class SettingsStateUIConfigBuilder;

class SettingsState : public State, public input::InputObserver
{
    friend class SettingsStateUIConfigBuilder;

public:
    explicit SettingsState(const std::shared_ptr<window::Window>&,
                           const std::shared_ptr<input::InputManager>&,
                           const std::shared_ptr<graphics::RendererPool>&,
                           std::stack<std::unique_ptr<State>>&, std::unique_ptr<components::ui::UIManager>);
    ~SettingsState();

    NextState update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;

private:
    void synchronizeWindowSettings();
    void applyWindowSettingsChanges();
    void refreshWindowSettingsUI();
    void increaseResolution();
    void decreaseResolution();
    void increaseFrameLimit();
    void decreaseFrameLimit();
    void switchVsync();
    void setWindowMode();
    void setFullscreenMode();

    bool shouldBackToMenu;
    window::WindowSettings selectedWindowsSettings;
    std::vector<window::Resolution> supportedResolutions;
    unsigned int selectedResolutionIndex = 0;
    std::vector<unsigned int> supportedFrameLimits;
    unsigned int selectedFrameLimitIndex = 0;
    const input::InputStatus* inputStatus;
    std::unique_ptr<components::ui::UIManager> uiManager;
};
}