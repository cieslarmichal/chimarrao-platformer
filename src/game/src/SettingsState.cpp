#include "SettingsState.h"

#include <utility>

#include "ProjectPathReader.h"
#include "SettingsStateUIConfigBuilder.h"
#include "ui/DefaultUIManager.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
}

SettingsState::SettingsState(const std::shared_ptr<window::Window>& windowInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                             std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                             std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)},
      iconNames{"settingsIcon1Image", "settingsIcon2Image", "settingsIcon3Image", "settingsIcon4Image",
                "settingsIcon5Image"}
{
    uiManager->createUI(SettingsStateUIConfigBuilder::createSettingsUIConfig(this));
    std::vector<std::vector<GridButtonInfo>> gridButtonsInfo{
        {GridButtonInfo{"settingsWindowModeButton", 0, true},
         GridButtonInfo{"settingsFullscreenModeButton", 0, true}},
        {GridButtonInfo{"settingsResolutionDecreaseButton", 1, true},
         GridButtonInfo{"settingsResolutionIncreaseButton", 1, true}},
        {GridButtonInfo{"settingsFrameLimitDecreaseButton", 3, true},
         GridButtonInfo{"settingsFrameLimitIncreaseButton", 3, true}},
        {GridButtonInfo{"settingsBackToMenuButton", 4, false},
         GridButtonInfo{"settingsApplyChangesButton", 4, false}}};
    uiNavigator = std::make_unique<GridButtonsNavigator>(*uiManager, gridButtonsInfo, iconNames, buttonColor,
                                                         buttonHoverColor);

    supportedResolutions = window->getSupportedResolutions();
    supportedFrameLimits = window->getSupportedFrameLimits();

    synchronizeWindowSettings();
    refreshWindowSettingsUI();
}

NextState SettingsState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiNavigator->update(deltaTime, input);
    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void SettingsState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void SettingsState::render()
{
    rendererPool->renderAll();
}

StateType SettingsState::getType() const
{
    return StateType::Settings;
}

void SettingsState::activate()
{
    active = true;
    synchronizeWindowSettings();
    uiManager->activate();
    uiNavigator->activate();
}

void SettingsState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void SettingsState::synchronizeWindowSettings()
{
    selectedWindowsSettings = window->getWindowSettings();

    if (const auto currentResolutionIter = std::find(supportedResolutions.begin(), supportedResolutions.end(),
                                                     selectedWindowsSettings.resolution);
        currentResolutionIter != supportedResolutions.cend())
    {
        selectedResolutionIndex =
            static_cast<unsigned int>(std::distance(supportedResolutions.begin(), currentResolutionIter));
    }

    if (const auto currentFrameLimitIter = std::find(supportedFrameLimits.begin(), supportedFrameLimits.end(),
                                                     selectedWindowsSettings.frameLimit);
        currentFrameLimitIter != supportedFrameLimits.cend())
    {
        selectedFrameLimitIndex =
            static_cast<unsigned int>(std::distance(supportedFrameLimits.begin(), currentFrameLimitIter));
    }
}

void SettingsState::applyWindowSettingsChanges()
{
    if (window->setDisplayMode(selectedWindowsSettings.displayMode))
    {
        rendererPool->synchronizeRenderingSize();
    }

    if (selectedWindowsSettings.displayMode != window::DisplayMode::Fullscreen)
    {
        if (window->setResolution(selectedWindowsSettings.resolution))
        {
            rendererPool->setRenderingSize(
                {selectedWindowsSettings.resolution.height, selectedWindowsSettings.resolution.width});
        }
    }

    window->setVerticalSync(selectedWindowsSettings.vsync);
    window->setFramerateLimit(selectedWindowsSettings.frameLimit);

    synchronizeWindowSettings();
    refreshWindowSettingsUI();
}

void SettingsState::refreshWindowSettingsUI()
{
    if (selectedWindowsSettings.displayMode == window::DisplayMode::Window)
    {
        uiManager->setColor("settingsWindowModeButton", buttonHoverColor);
    }
    else
    {
        uiManager->setColor("settingsFullscreenModeButton", buttonHoverColor);
    }

    uiManager->setText("settingsResolutionValueLabel", toString(selectedWindowsSettings.resolution));
    uiManager->setText("settingsFrameLimitValueLabel", std::to_string(selectedWindowsSettings.frameLimit));
    uiManager->setChecked("settingsVsyncCheckBox", selectedWindowsSettings.vsync);
}

void SettingsState::increaseResolution()
{
    if (selectedResolutionIndex == supportedResolutions.size() - 1)
    {
        selectedResolutionIndex = 0;
    }
    else
    {
        selectedResolutionIndex++;
    }

    selectedWindowsSettings.resolution = supportedResolutions[selectedResolutionIndex];
    uiManager->setText("settingsResolutionValueLabel", toString(selectedWindowsSettings.resolution));
}

void SettingsState::decreaseResolution()
{
    if (selectedResolutionIndex == 0)
    {
        selectedResolutionIndex = static_cast<unsigned int>(supportedResolutions.size() - 1);
    }
    else
    {
        selectedResolutionIndex--;
    }

    selectedWindowsSettings.resolution = supportedResolutions[selectedResolutionIndex];
    uiManager->setText("settingsResolutionValueLabel", toString(selectedWindowsSettings.resolution));
}

void SettingsState::increaseFrameLimit()
{
    if (selectedFrameLimitIndex == supportedFrameLimits.size() - 1)
    {
        selectedFrameLimitIndex = 0;
    }
    else
    {
        selectedFrameLimitIndex++;
    }

    selectedWindowsSettings.frameLimit = supportedFrameLimits[selectedFrameLimitIndex];
    uiManager->setText("settingsFrameLimitValueLabel", std::to_string(selectedWindowsSettings.frameLimit));
}

void SettingsState::decreaseFrameLimit()
{
    if (selectedFrameLimitIndex == 0)
    {
        selectedFrameLimitIndex = static_cast<unsigned int>(supportedFrameLimits.size() - 1);
    }
    else
    {
        selectedFrameLimitIndex--;
    }

    selectedWindowsSettings.frameLimit = supportedFrameLimits[selectedFrameLimitIndex];
    uiManager->setText("settingsFrameLimitValueLabel", std::to_string(selectedWindowsSettings.frameLimit));
}

void SettingsState::switchVsync()
{
    if (selectedWindowsSettings.vsync)
    {
        selectedWindowsSettings.vsync = false;
        uiManager->setChecked("settingsVsyncCheckBox", false);
    }
    else
    {
        selectedWindowsSettings.vsync = true;
        uiManager->setChecked("settingsVsyncCheckBox", true);
    }
}

void SettingsState::setWindowMode()
{
    selectedWindowsSettings.displayMode = window::DisplayMode::Window;
    uiManager->setColor("settingsWindowModeButton", buttonHoverColor);
    uiManager->setColor("settingsFullscreenModeButton", buttonColor);
}

void SettingsState::setFullscreenMode()
{
    selectedWindowsSettings.displayMode = window::DisplayMode::Fullscreen;
    uiManager->setColor("settingsWindowModeButton", buttonColor);
    uiManager->setColor("settingsFullscreenModeButton", buttonHoverColor);
}
}