#include "SettingsState.h"

#include "GetProjectPath.h"
#include "SettingsStateUIConfigBuilder.h"
#include "StlOperators.h"
#include "ui/DefaultUIManager.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
}

// TODO: pass uiManager in constructor
SettingsState::SettingsState(const std::shared_ptr<window::Window>& windowInit,
                             const std::shared_ptr<input::InputManager>& inputManagerInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                             std::stack<std::unique_ptr<State>>& statesInit,
                             std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      shouldBackToMenu{false},
      inputStatus{nullptr},
      uiManager{std::move(uiManagerInit)}
{
    inputManager->registerObserver(this);
    uiManager->createUI(SettingsStateUIConfigBuilder::createSettingsUIConfig(this));

    supportedResolutions = window->getSupportedResolutions();
    supportedFrameLimits = window->getSupportedFrameLimits();
    // TODO: throw if supported resolution or frameLimits == 0

    synchronizeWindowSettings();

    if (selectedWindowsSettings.displayMode == window::DisplayMode::Window)
    {
        uiManager->setColor(components::ui::UIComponentType::Button, "settingsWindowModeButton",
                            buttonHoverColor);
    }
    else
    {
        uiManager->setColor(components::ui::UIComponentType::Button, "settingsFullscreenModeButton",
                            buttonHoverColor);
    }

    uiManager->setText(components::ui::UIComponentTypeWithLabel::Label, "settingsResolutionValueLabel",
                       toString(selectedWindowsSettings.resolution));
    uiManager->setText(components::ui::UIComponentTypeWithLabel::Label, "settingsFrameLimitValueLabel",
                       std::to_string(selectedWindowsSettings.frameLimit));
    uiManager->setChecked(components::ui::UIComponentTypeWithCheck::CheckBox, "settingsVsyncCheckBox",
                          selectedWindowsSettings.vsync);
}

SettingsState::~SettingsState()
{
    inputManager->removeObserver(this);
}

NextState SettingsState::update(const utils::DeltaTime& deltaTime)
{
    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime);
    return NextState::Same;
}

void SettingsState::lateUpdate(const utils::DeltaTime&) {}

void SettingsState::render()
{
    rendererPool->renderAll();
}

std::string SettingsState::getName() const
{
    return "Settings state";
}

void SettingsState::activate()
{
    active = true;
    synchronizeWindowSettings();
    uiManager->activate();
}

void SettingsState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void SettingsState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void SettingsState::synchronizeWindowSettings()
{
    selectedWindowsSettings = window->getWindowSettings();

    if (const auto currentResolutionIter = std::find(supportedResolutions.begin(), supportedResolutions.end(),
                                                     selectedWindowsSettings.resolution);
        currentResolutionIter != supportedResolutions.cend())
    {
        selectedResolutionIndex = std::distance(supportedResolutions.begin(), currentResolutionIter);
    }

    if (const auto currentFrameLimitIter = std::find(supportedFrameLimits.begin(), supportedFrameLimits.end(),
                                                     selectedWindowsSettings.frameLimit);
        currentFrameLimitIter != supportedFrameLimits.cend())
    {
        selectedFrameLimitIndex = std::distance(supportedFrameLimits.begin(), currentFrameLimitIter);
    }
}

void SettingsState::applyWindowSettingsChanges()
{
    if (window->setDisplayMode(selectedWindowsSettings.displayMode))
    {
        rendererPool->synchronizeRenderingSize();
    }

    if (window->setResolution(selectedWindowsSettings.resolution))
    {
        rendererPool->setRenderingSize(
            {selectedWindowsSettings.resolution.height, selectedWindowsSettings.resolution.width});
    }

    window->setVerticalSync(selectedWindowsSettings.vsync);
    window->setFramerateLimit(selectedWindowsSettings.frameLimit);
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
    uiManager->setText(components::ui::UIComponentTypeWithLabel::Label, "settingsResolutionValueLabel",
                       toString(selectedWindowsSettings.resolution));
}

void SettingsState::decreaseResolution()
{
    if (selectedResolutionIndex == 0)
    {
        selectedResolutionIndex = supportedResolutions.size() - 1;
    }
    else
    {
        selectedResolutionIndex--;
    }

    selectedWindowsSettings.resolution = supportedResolutions[selectedResolutionIndex];
    uiManager->setText(components::ui::UIComponentTypeWithLabel::Label, "settingsResolutionValueLabel",
                       toString(selectedWindowsSettings.resolution));
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
    uiManager->setText(components::ui::UIComponentTypeWithLabel::Label, "settingsFrameLimitValueLabel",
                       std::to_string(selectedWindowsSettings.frameLimit));
}

void SettingsState::decreaseFrameLimit()
{
    if (selectedFrameLimitIndex == 0)
    {
        selectedFrameLimitIndex = supportedFrameLimits.size() - 1;
    }
    else
    {
        selectedFrameLimitIndex--;
    }

    selectedWindowsSettings.frameLimit = supportedFrameLimits[selectedFrameLimitIndex];
    uiManager->setText(components::ui::UIComponentTypeWithLabel::Label, "settingsFrameLimitValueLabel",
                       std::to_string(selectedWindowsSettings.frameLimit));
}

void SettingsState::switchVsync()
{
    if (selectedWindowsSettings.vsync)
    {
        selectedWindowsSettings.vsync = false;
        uiManager->setChecked(components::ui::UIComponentTypeWithCheck::CheckBox, "settingsVsyncCheckBox",
                              false);
    }
    else
    {
        selectedWindowsSettings.vsync = true;
        uiManager->setChecked(components::ui::UIComponentTypeWithCheck::CheckBox, "settingsVsyncCheckBox",
                              true);
    }
}

void SettingsState::setWindowMode()
{
    selectedWindowsSettings.displayMode = window::DisplayMode::Window;
    uiManager->setColor(components::ui::UIComponentType::Button, "settingsWindowModeButton",
                        buttonHoverColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "settingsFullscreenModeButton", buttonColor);
}

void SettingsState::setFullscreenMode()
{
    selectedWindowsSettings.displayMode = window::DisplayMode::Fullscreen;
    uiManager->setColor(components::ui::UIComponentType::Button, "settingsWindowModeButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "settingsFullscreenModeButton",
                        buttonHoverColor);
}

}