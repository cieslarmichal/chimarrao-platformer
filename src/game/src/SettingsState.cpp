#include "SettingsState.h"

#include "GetProjectPath.h"
#include "StlOperators.h"
#include "core/ClickableComponent.h"
#include "core/GraphicsComponent.h"
#include "core/HitboxComponent.h"
#include "core/MouseOverComponent.h"
#include "core/TextComponent.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto buttonSize = utils::Vector2f{25, 5};
const auto sectionTextFontSize{37};
const auto displayModeFontSize{12};
const auto displayModeButtonSize = utils::Vector2f{8, 3};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
const auto backgroundPath =
    utils::getProjectPath("chimarrao-platformer") + "resources/BG/menu_background.jpg";

const auto settingsTitlePosition = utils::Vector2f{32, 6};
const auto displayModeSectionPosition = utils::Vector2f{38, 18};
const auto windowModeButtonPosition = utils::Vector2f{
    displayModeSectionPosition.x + 15.8f + displayModeButtonSize.x, displayModeSectionPosition.y + 0.5f};
const auto fullscreenModeButtonPosition =
    utils::Vector2f{windowModeButtonPosition.x + displayModeButtonSize.x + 0.5f, windowModeButtonPosition.y};
const auto resolutionSectionPosition = utils::Vector2f{38, 24};
const auto resolutionTextFieldPosition = utils::Vector2f{
    resolutionSectionPosition.x + 18.50f + displayModeButtonSize.x, resolutionSectionPosition.y + 0.8f};
const auto changeResolutionButtonSize = utils::Vector2f{2.5, 2};
const auto resolutionDecreaseButtonPosition =
    utils::Vector2f{resolutionTextFieldPosition.x - 2.8f, resolutionSectionPosition.y + 1.15f};
const auto resolutionIncreaseButtonPosition =
    utils::Vector2f{resolutionTextFieldPosition.x + 11.2f, resolutionSectionPosition.y + 1.15f};
const auto vsyncSectionPosition = utils::Vector2f{38, 30};
const auto vsyncButtonPosition =
    utils::Vector2f{vsyncSectionPosition.x + 31.0f, vsyncSectionPosition.y + 0.5f};
const auto vsyncButtonSize = utils::Vector2f{3, 3};
const auto frameLimitSectionPosition = utils::Vector2f{38, 36};
const auto frameLimitTextFieldPosition = utils::Vector2f{
    frameLimitSectionPosition.x + 22.5f + displayModeButtonSize.x, frameLimitSectionPosition.y + 0.8f};
const auto frameLimitDecreaseButtonPosition =
    utils::Vector2f{frameLimitTextFieldPosition.x - 4.0f, frameLimitSectionPosition.y + 1.15f};
const auto frameLimitIncreaseButtonPosition =
    utils::Vector2f{frameLimitTextFieldPosition.x + 4.5f, frameLimitSectionPosition.y + 1.15f};
const auto backToMenuButtonPosition = utils::Vector2f{34.5, 48};
const auto applyChangesButtonPosition = utils::Vector2f{55, 48};
}

SettingsState::SettingsState(const std::shared_ptr<window::Window>& windowInit,
                             const std::shared_ptr<input::InputManager>& inputManagerInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                             std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      shouldBackToMenu{false},
      inputStatus{nullptr},
      timeAfterButtonsCanBeClicked{0.3f}
{
    inputManager->registerObserver(this);

    supportedResolutions = window->getSupportedResolutions();
    supportedFrameLimits = window->getSupportedFrameLimits();
    // TODO: throw if supported resolution or frameLimits == 0

    synchronizeWindowSettings();

    createBackground();
    createSettingsTitle();
    createBackToMenuButton();
    createApplyChangesButton();
    createDisplayModeSection();
    createResolutionSection();
    createVsyncSection();
    createFrameLimitSection();

    initialize();
}

SettingsState::~SettingsState()
{
    inputManager->removeObserver(this);
}

void SettingsState::initialize()
{
    for (auto& button : buttons)
    {
        button->loadDependentComponents();
        button->start();
        button->getComponent<components::core::ClickableComponent>()->disable();
    }
}

void SettingsState::update(const utils::DeltaTime& deltaTime)
{
    if (buttonsActionsFrozen &&
        freezeClickableButtonsTimer.getElapsedSeconds() > timeAfterButtonsCanBeClicked)
    {
        unfreezeButtons();
    }

    if (shouldBackToMenu)
    {
        backToMenu();
        return;
    }

    for (auto& button : buttons)
    {
        button->update(deltaTime);
    }
}

void SettingsState::lateUpdate(const utils::DeltaTime& deltaTime)
{
    for (auto& button : buttons)
    {
        button->lateUpdate(deltaTime);
    }
}

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

    for (auto& button : buttons)
    {
        button->enable();
    }
}

void SettingsState::deactivate()
{
    active = false;

    for (auto& button : buttons)
    {
        button->disable();
    }
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

void SettingsState::unfreezeButtons()
{
    buttonsActionsFrozen = false;
    for (auto& button : buttons)
    {
        button->getComponent<components::core::ClickableComponent>()->enable();
    }
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
    texts[resolutionTextId]->getComponent<components::core::TextComponent>()->setText(
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
    texts[resolutionTextId]->getComponent<components::core::TextComponent>()->setText(
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
    texts[frameLimitTextId]->getComponent<components::core::TextComponent>()->setText(
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
    texts[frameLimitTextId]->getComponent<components::core::TextComponent>()->setText(
        std::to_string(selectedWindowsSettings.frameLimit));
}

void SettingsState::switchVsync()
{
    if (selectedWindowsSettings.vsync)
    {
        selectedWindowsSettings.vsync = false;
        buttons[vsyncButtonId]->getComponent<components::core::TextComponent>()->setText("");
    }
    else
    {
        selectedWindowsSettings.vsync = true;
        buttons[vsyncButtonId]->getComponent<components::core::TextComponent>()->setText("X");
    }
}

void SettingsState::setWindowMode()
{
    selectedWindowsSettings.displayMode = window::DisplayMode::Window;

    buttons[windowModeButtonId]->getComponent<components::core::GraphicsComponent>()->setColor(
        buttonHoverColor);
    buttons[fullscreenModeButtonId]->getComponent<components::core::GraphicsComponent>()->setColor(
        buttonColor);
}

void SettingsState::setFullscreenMode()
{
    selectedWindowsSettings.displayMode = window::DisplayMode::Fullscreen;

    buttons[windowModeButtonId]->getComponent<components::core::GraphicsComponent>()->setColor(buttonColor);
    buttons[fullscreenModeButtonId]->getComponent<components::core::GraphicsComponent>()->setColor(
        buttonHoverColor);
}

void SettingsState::backToMenu()
{
    states.pop();

    if (not states.empty())
    {
        states.top()->activate();
    }
}

void SettingsState::createBackground()
{
    background = std::make_unique<components::core::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::core::GraphicsComponent>(rendererPool, utils::Vector2f{80, 60},
                                                                  utils::Vector2f{0, 0}, backgroundPath,
                                                                  graphics::VisibilityLayer::Background);
}

void SettingsState::createSettingsTitle()
{
    addText(settingsTitlePosition, "Settings", 37);
}

void SettingsState::createBackToMenuButton()
{
    const auto backToMenuButtonSize = utils::Vector2f{13, 5};
    addButtonWithMouseOver(backToMenuButtonPosition, backToMenuButtonSize, "Back", sectionTextFontSize,
                           utils::Vector2f{2, 0}, [this] { shouldBackToMenu = true; });
}

void SettingsState::createApplyChangesButton()
{
    const auto applyChangesButtonSize = utils::Vector2f{13, 5};
    addButtonWithMouseOver(applyChangesButtonPosition, applyChangesButtonSize, "Apply", sectionTextFontSize,
                           utils::Vector2f{1, 0}, [this] { applyWindowSettingsChanges(); });
}

void SettingsState::createDisplayModeSection()
{
    addText(displayModeSectionPosition, "Display mode:", 30);
    windowModeButtonId = addButton(windowModeButtonPosition, displayModeButtonSize, "Window",
                                   displayModeFontSize, utils::Vector2f{2, 0.9}, [this] { setWindowMode(); });
    fullscreenModeButtonId =
        addButton(fullscreenModeButtonPosition, displayModeButtonSize, "Fullscreen", displayModeFontSize,
                  utils::Vector2f{0.5, 0.9}, [this] { setFullscreenMode(); });

    if (selectedWindowsSettings.displayMode == window::DisplayMode::Window)
    {
        buttons[windowModeButtonId]->getComponent<components::core::GraphicsComponent>()->setColor(
            buttonHoverColor);
    }
    else
    {
        buttons[fullscreenModeButtonId]->getComponent<components::core::GraphicsComponent>()->setColor(
            buttonHoverColor);
    }
}

void SettingsState::createResolutionSection()
{
    addText(resolutionSectionPosition, "Resolution:", 30);
    resolutionTextId =
        addText(resolutionTextFieldPosition, toString(supportedResolutions[selectedResolutionIndex]), 20);
    addButtonWithMouseOver(resolutionDecreaseButtonPosition, changeResolutionButtonSize, "<", 20,
                           utils::Vector2f{0.6, -0.3}, [this] { decreaseResolution(); });
    addButtonWithMouseOver(resolutionIncreaseButtonPosition, changeResolutionButtonSize, ">", 20,
                           utils::Vector2f{0.6, -0.3}, [this] { increaseResolution(); });
}

void SettingsState::createVsyncSection()
{
    addText(vsyncSectionPosition, "Vsync:", 30);
    const std::string vsyncText = selectedWindowsSettings.vsync ? "X" : "";
    vsyncButtonId = addButtonWithMouseOver(vsyncButtonPosition, vsyncButtonSize, vsyncText, 25,
                                           utils::Vector2f{0.8, 0.0}, [this] { switchVsync(); });
}

void SettingsState::createFrameLimitSection()
{
    addText(frameLimitSectionPosition, "Frame limit:", 30);
    frameLimitTextId = addText(frameLimitTextFieldPosition,
                               std::to_string(supportedFrameLimits[selectedFrameLimitIndex]), 20);
    addButtonWithMouseOver(frameLimitDecreaseButtonPosition, changeResolutionButtonSize, "<", 20,
                           utils::Vector2f{0.6, -0.3}, [this] { decreaseFrameLimit(); });
    addButtonWithMouseOver(frameLimitIncreaseButtonPosition, changeResolutionButtonSize, ">", 20,
                           utils::Vector2f{0.6, -0.3}, [this] { increaseFrameLimit(); });
}

unsigned int SettingsState::addButton(const utils::Vector2f& position, const utils::Vector2f& size,
                                      const std::string& text, unsigned int fontSize,
                                      const utils::Vector2f& textOffset,
                                      std::function<void(void)> clickAction)
{
    auto buttonId = buttons.size();
    auto button = std::make_unique<components::core::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::core::GraphicsComponent>(
        rendererPool, size, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::core::TextComponent>(rendererPool, position, text, fontPath, fontSize,
                                                          graphics::Color::Black, textOffset);
    button->addComponent<components::core::HitboxComponent>(size);
    button->addComponent<components::core::ClickableComponent>(inputManager, std::move(clickAction));

    buttons.push_back(std::move(button));
    return buttonId;
}

unsigned int SettingsState::addButtonWithMouseOver(const utils::Vector2f& position,
                                                   const utils::Vector2f& size, const std::string& text,
                                                   unsigned int fontSize, const utils::Vector2f& textOffset,
                                                   std::function<void(void)> clickAction)
{
    auto buttonId = buttons.size();
    auto button = std::make_unique<components::core::ComponentOwner>(position);
    auto graphicsComponent = button->addComponent<components::core::GraphicsComponent>(
        rendererPool, size, position, buttonColor, graphics::VisibilityLayer::First);
    button->addComponent<components::core::TextComponent>(rendererPool, position, text, fontPath, fontSize,
                                                          graphics::Color::Black, textOffset);
    button->addComponent<components::core::HitboxComponent>(size);
    button->addComponent<components::core::ClickableComponent>(inputManager, std::move(clickAction));

    const auto changeColorOnMouseOver = [=] { graphicsComponent->setColor(buttonHoverColor); };
    const auto changeColorOnMouseOut = [=] { graphicsComponent->setColor(buttonColor); };
    button->addComponent<components::core::MouseOverComponent>(inputManager, changeColorOnMouseOver,
                                                               changeColorOnMouseOut);
    buttons.push_back(std::move(button));
    return buttonId;
}

unsigned int SettingsState::addText(const utils::Vector2f& position, const std::string& description,
                                    unsigned int fontSize)
{
    auto textId = texts.size();
    auto text = std::make_unique<components::core::ComponentOwner>(position);
    text->addComponent<components::core::TextComponent>(rendererPool, position, description, fontPath,
                                                        fontSize, graphics::Color::Black);
    texts.push_back(std::move(text));
    return textId;
}

}