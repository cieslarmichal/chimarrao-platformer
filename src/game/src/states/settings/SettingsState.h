#pragma once

#include "ComponentOwner.h"
#include "State.h"
#include "Timer.h"
#include "UIConfig.h"
#include "UIManager.h"
#include "navigators/GridButtonsNavigator.h"

namespace game
{
class SettingsStateUIConfigBuilder;

class SettingsState : public State
{
    friend class SettingsStateUIConfigBuilder;

public:
    explicit SettingsState(const std::shared_ptr<window::Window>&,
                           const std::shared_ptr<graphics::RendererPool>&, std::shared_ptr<utils::FileAccess>,
                           States&, std::shared_ptr<components::ui::UIManager>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    void backToMenu();
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
    std::shared_ptr<components::ui::UIManager> uiManager;
    std::unique_ptr<GridButtonsNavigator> buttonsNavigator;
};
}