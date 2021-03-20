#include "MenuState.h"

#include "ControlsState.h"
#include "EditorState.h"
#include "GameState.h"
#include "GetProjectPath.h"
#include "SettingsState.h"
#include "core/GraphicsComponent.h"
#include "core/HitBoxComponent.h"
#include "core/MouseOverComponent.h"
#include "ui/DefaultUIManager.h"

namespace game
{
namespace
{
const auto gameButtonPosition = utils::Vector2f{50, 7};
const auto mapEditorButtonPosition = utils::Vector2f{50, 17};
const auto controlsButtonPosition = utils::Vector2f{50, 27};
const auto settingsButtonPosition = utils::Vector2f{50, 37};
const auto exitButtonPosition = utils::Vector2f{50, 47};
const std::vector<utils::Vector2f> buttonsPositions{gameButtonPosition, mapEditorButtonPosition,
                                                    controlsButtonPosition, settingsButtonPosition,
                                                    exitButtonPosition};
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto buttonSize = utils::Vector2f{23, 6};
const auto iconSize = utils::Vector2f{4, 4};
const auto fontPath = utils::getProjectPath("chimarrao-platformer") + "resources/fonts/VeraMono.ttf";
const auto iconPath = utils::getProjectPath("chimarrao-platformer") + "resources/yerba_item.png";
const auto backgroundPath =
    utils::getProjectPath("chimarrao-platformer") + "resources/BG/menu_background.jpg";
}

MenuState::MenuState(const std::shared_ptr<window::Window>& windowInit,
                     const std::shared_ptr<input::InputManager>& inputManagerInit,
                     const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                     std::stack<std::unique_ptr<State>>& statesInit)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      currentButtonIndex{0},
      timeAfterButtonCanBeSwitched{0.1},
      buttonNames{"menuPlayButton", "menuMapEditorButton", "menuControlsButton", "menuSettingsButton",
                  "menuExitButton"},
      uiManager{std::make_unique<components::ui::DefaultUIManager>(inputManagerInit, rendererPoolInit,
                                                                   createSettingsUIConfig())}
{
    inputManager->registerObserver(this);
    createIcons();
    uiManager->setColor(components::ui::UIComponentType::Button, buttonNames.at(currentButtonIndex),
                        buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

MenuState::~MenuState()
{
    inputManager->removeObserver(this);
}

void MenuState::update(const utils::DeltaTime& deltaTime)
{
    if (switchButtonTimer.getElapsedSeconds() > timeAfterButtonCanBeSwitched)
    {
        handleButtonSwitching();
    }

    uiManager->update(deltaTime);
}

void MenuState::lateUpdate(const utils::DeltaTime& deltaTime)
{
}

void MenuState::render()
{
    rendererPool->renderAll();
}

std::string MenuState::getName() const
{
    return "Menu state";
}

void MenuState::activate()
{
    active = true;
    uiManager->activate();
}

void MenuState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void MenuState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void MenuState::handleButtonSwitching()
{
    if (inputStatus->isKeyPressed(input::InputKey::Up))
    {
        changeSelectedButtonUp();
    }
    else if (inputStatus->isKeyPressed(input::InputKey::Down))
    {
        changeSelectedButtonDown();
    }
    switchButtonTimer.restart();
}

void MenuState::createIcons()
{
    for (const auto& buttonPosition : buttonsPositions)
    {
        const auto iconPosition = utils::Vector2f{buttonPosition.x + buttonSize.x, buttonPosition.y + 1};
        addIcon(iconPosition);
    }
}

void MenuState::addIcon(const utils::Vector2f& position)
{
    auto icon = std::make_unique<components::core::ComponentOwner>(
        position, "icon" + std::to_string(position.x) + std::to_string(position.y));
    icon->addComponent<components::core::GraphicsComponent>(rendererPool, iconSize, position, iconPath,
                                                            graphics::VisibilityLayer::First);
    icons.push_back(std::move(icon));
}

void MenuState::changeSelectedButtonUp()
{
    unselectAllButtons();

    if (currentButtonIndex == 0)
    {
        currentButtonIndex = buttonNames.size() - 1;
    }
    else
    {
        currentButtonIndex--;
    }
    uiManager->setColor(components::ui::UIComponentType::Button, buttonNames.at(currentButtonIndex),
                        buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

void MenuState::changeSelectedButtonDown()
{
    unselectAllButtons();

    if (currentButtonIndex == buttonNames.size() - 1)
    {
        currentButtonIndex = 0;
    }
    else
    {
        currentButtonIndex++;
    }
    uiManager->setColor(components::ui::UIComponentType::Button, buttonNames.at(currentButtonIndex),
                        buttonHoverColor);
    setIconVisible(currentButtonIndex);
}

void MenuState::changeSelectedButton(unsigned int buttonIndex)
{
    currentButtonIndex = buttonIndex;
    setIconVisible(currentButtonIndex);
}

void MenuState::unselectAllButtons()
{
    uiManager->setColor(components::ui::UIComponentType::Button, "menuPlayButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuMapEditorButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuControlsButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuSettingsButton", buttonColor);
    uiManager->setColor(components::ui::UIComponentType::Button, "menuExitButton", buttonColor);
}

void MenuState::setIconVisible(unsigned int iconIndex)
{
    hideIcons();
    icons[iconIndex]->getComponent<components::core::GraphicsComponent>()->setVisibility(
        graphics::VisibilityLayer::First);
}

void MenuState::hideIcons()
{
    for (auto& icon : icons)
    {
        icon->getComponent<components::core::GraphicsComponent>()->setVisibility(
            graphics::VisibilityLayer::Invisible);
    }
}

std::unique_ptr<components::ui::UIConfig> MenuState::createSettingsUIConfig()
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;
    std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> checkBoxesConfig;
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;
    std::vector<std::unique_ptr<components::ui::TextFieldConfig>> textFieldsConfig;

    auto backgroundConfig = std::make_unique<components::ui::BackgroundConfig>(
        "menuBackground", utils::Vector2f{0, 0}, utils::Vector2f{80, 60},
        graphics::VisibilityLayer::Background, backgroundPath);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto playButtonOnMouseOver = [=] {
        unselectAllButtons();
        changeSelectedButton(0);
        uiManager->setColor(components::ui::UIComponentType::Button, "menuPlayButton", buttonHoverColor);
    };
    const auto playButtonOnMouseOut = [=] {
        unselectAllButtons();
        hideIcons();
        uiManager->setColor(components::ui::UIComponentType::Button, "menuPlayButton", buttonColor);
    };
    auto playButtonMouseOverActions =
        components::ui::MouseOverActions{playButtonOnMouseOver, playButtonOnMouseOut};
    const auto runGame = [&] {
        states.top()->deactivate();
        states.push(std::make_unique<GameState>(window, inputManager, rendererPool, states));
    };
    auto playButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuPlayButton", gameButtonPosition, buttonSize, buttonColor, "Play", graphics::Color::Black, 35,
        fontPath, utils::Vector2f{7, 1}, runGame, playButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(playButtonConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto mapEditorButtonOnMouseOver = [=] {
        unselectAllButtons();
        changeSelectedButton(1);
        uiManager->setColor(components::ui::UIComponentType::Button, "menuMapEditorButton", buttonHoverColor);
    };
    const auto mapEditorButtonOnMouseOut = [=] {
        unselectAllButtons();
        hideIcons();
        uiManager->setColor(components::ui::UIComponentType::Button, "menuMapEditorButton", buttonColor);
    };
    auto mapEditorButtonMouseOverActions =
        components::ui::MouseOverActions{mapEditorButtonOnMouseOver, mapEditorButtonOnMouseOut};
    const auto runMapEditor = [&] {
        states.top()->deactivate();
        states.push(std::make_unique<EditorState>(window, inputManager, rendererPool, states));
    };
    auto mapEditorButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuMapEditorButton", mapEditorButtonPosition, buttonSize, buttonColor, "Map Editor",
        graphics::Color::Black, 35, fontPath, utils::Vector2f{1, 1}, runMapEditor,
        mapEditorButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(mapEditorButtonConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto controlsButtonOnMouseOver = [=] {
        unselectAllButtons();
        changeSelectedButton(2);
        uiManager->setColor(components::ui::UIComponentType::Button, "menuControlsButton", buttonHoverColor);
    };
    const auto controlsButtonOnMouseOut = [=] {
        unselectAllButtons();
        hideIcons();
        uiManager->setColor(components::ui::UIComponentType::Button, "menuControlsButton", buttonColor);
    };
    auto controlsButtonMouseOverActions =
        components::ui::MouseOverActions{controlsButtonOnMouseOver, controlsButtonOnMouseOut};
    const auto runControls = [&] {
        states.top()->deactivate();
        states.push(std::make_unique<ControlsState>(window, inputManager, rendererPool, states));
    };
    auto controlsButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuControlsButton", controlsButtonPosition, buttonSize, buttonColor, "Controls",
        graphics::Color::Black, 35, fontPath, utils::Vector2f{3, 1}, runControls,
        controlsButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(controlsButtonConfig));

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    const auto settingsButtonOnMouseOver = [=] {
        unselectAllButtons();
        changeSelectedButton(3);
        uiManager->setColor(components::ui::UIComponentType::Button, "menuSettingsButton", buttonHoverColor);
    };
    const auto settingsButtonOnMouseOut = [=] {
        unselectAllButtons();
        hideIcons();
        uiManager->setColor(components::ui::UIComponentType::Button, "menuSettingsButton", buttonColor);
    };
    auto settingsButtonMouseOverActions =
        components::ui::MouseOverActions{settingsButtonOnMouseOver, settingsButtonOnMouseOut};
    const auto runSettings = [&] {
        states.top()->deactivate();
        states.push(std::make_unique<SettingsState>(window, inputManager, rendererPool, states));
    };
    auto settingsButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuSettingsButton", settingsButtonPosition, buttonSize, buttonColor, "Settings",
        graphics::Color::Black, 35, fontPath, utils::Vector2f{3, 1}, runSettings,
        settingsButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(settingsButtonConfig));

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    const auto exitButtonOnMouseOver = [=] {
        unselectAllButtons();
        changeSelectedButton(4);
        uiManager->setColor(components::ui::UIComponentType::Button, "menuExitButton", buttonHoverColor);
    };
    const auto exitButtonOnMouseOut = [=] {
        unselectAllButtons();
        hideIcons();
        uiManager->setColor(components::ui::UIComponentType::Button, "menuExitButton", buttonColor);
    };
    auto exitButtonMouseOverActions =
        components::ui::MouseOverActions{exitButtonOnMouseOver, exitButtonOnMouseOut};
    const auto exit = [&] { window->close(); };
    auto exitButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuExitButton", exitButtonPosition, buttonSize, buttonColor, "Exit", graphics::Color::Black, 35,
        fontPath, utils::Vector2f{7, 1}, exit, exitButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(exitButtonConfig));

    return std::make_unique<components::ui::UIConfig>(std::move(backgroundConfig), std::move(buttonsConfig),
                                                      std::move(checkBoxesConfig), std::move(labelsConfig),
                                                      std::move(textFieldsConfig));
}

}