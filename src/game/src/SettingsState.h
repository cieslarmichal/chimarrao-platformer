#pragma once

#include "InputObserver.h"
#include "State.h"
#include "Timer.h"
#include "core/ComponentOwner.h"
#include "ui/UIManager.h"
#include "ui/UIConfig.h"

namespace game
{
class SettingsState : public State
{
public:
    explicit SettingsState(const std::shared_ptr<window::Window>&,
                           const std::shared_ptr<input::InputManager>&,
                           const std::shared_ptr<graphics::RendererPool>&,
                           std::stack<std::unique_ptr<State>>&);

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;

private:
    void synchronizeWindowSettings();
    void applyWindowSettingsChanges();
    void increaseResolution();
    void decreaseResolution();
    void increaseFrameLimit();
    void decreaseFrameLimit();
    void switchVsync();
    void setWindowMode();
    void setFullscreenMode();
    void backToMenu();
    std::unique_ptr<components::ui::UIConfig> createSettingsUIConfig();

    bool shouldBackToMenu;
    window::WindowSettings selectedWindowsSettings;
    std::vector<window::Resolution> supportedResolutions;
    unsigned int selectedResolutionIndex = 0;
    std::vector<unsigned int> supportedFrameLimits;
    unsigned int selectedFrameLimitIndex = 0;
    std::unique_ptr<components::ui::UIManager> uiManager;
};
}