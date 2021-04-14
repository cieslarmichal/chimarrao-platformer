#include "MenuStateUIConfigBuilder.h"

#include "CommonUIConfigElements.h"
#include "ControlsState.h"
#include "EditorState.h"
#include "GameState.h"
#include "GetProjectPath.h"
#include "MenuState.h"
#include "SettingsState.h"
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
}

std::unique_ptr<components::ui::UIConfig> MenuStateUIConfigBuilder::createMenuUIConfig(MenuState* menuState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(menuState), std::move(createButtonConfigs(menuState)),
        createCheckBoxConfigs(menuState), createLabelConfigs(menuState), createTextFieldConfigs(menuState));
}

std::unique_ptr<components::ui::BackgroundConfig> MenuStateUIConfigBuilder::createBackgroundConfig(MenuState*)
{
    return std::make_unique<components::ui::BackgroundConfig>(
        "menuBackground", utils::Vector2f{0, 0}, utils::Vector2f{80, 60},
        graphics::VisibilityLayer::Background, menuBackgroundPath);
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
MenuStateUIConfigBuilder::createButtonConfigs(MenuState* menuState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;

    const auto playButtonOnMouseOver = [=]
    {
        menuState->unselectAllButtons();
        menuState->changeSelectedButton(0);
        menuState->uiManager->setColor(components::ui::UIComponentType::Button, "menuPlayButton",
                                       buttonHoverColor);
    };
    const auto playButtonOnMouseOut = [=]
    {
        menuState->unselectAllButtons();
        menuState->hideIcons();
        menuState->uiManager->setColor(components::ui::UIComponentType::Button, "menuPlayButton",
                                       buttonColor);
    };
    auto playButtonMouseOverActions =
        components::ui::MouseOverActions{playButtonOnMouseOver, playButtonOnMouseOut};
    const auto runGame = [=]
    {
        menuState->states.top()->deactivate();
        menuState->states.push(std::make_unique<GameState>(
            menuState->window, menuState->inputManager, menuState->rendererPool, menuState->states,
            std::make_unique<components::ui::DefaultUIManager>(menuState->inputManager,
                                                               menuState->rendererPool)));
    };
    auto playButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuPlayButton", gameButtonPosition, buttonSize, buttonColor, "Play", graphics::Color::Black, 35,
        fontPath, utils::Vector2f{7, 1}, runGame, playButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(playButtonConfig));

    const auto mapEditorButtonOnMouseOver = [=]
    {
        menuState->unselectAllButtons();
        menuState->changeSelectedButton(1);
        menuState->uiManager->setColor(components::ui::UIComponentType::Button, "menuMapEditorButton",
                                       buttonHoverColor);
    };
    const auto mapEditorButtonOnMouseOut = [=]
    {
        menuState->unselectAllButtons();
        menuState->hideIcons();
        menuState->uiManager->setColor(components::ui::UIComponentType::Button, "menuMapEditorButton",
                                       buttonColor);
    };
    auto mapEditorButtonMouseOverActions =
        components::ui::MouseOverActions{mapEditorButtonOnMouseOver, mapEditorButtonOnMouseOut};
    const auto runMapEditor = [=]
    {
        menuState->states.top()->deactivate();
        menuState->states.push(std::make_unique<EditorState>(
            menuState->window, menuState->inputManager, menuState->rendererPool, menuState->states,
            std::make_unique<components::ui::DefaultUIManager>(menuState->inputManager,
                                                               menuState->rendererPool)));
    };
    auto mapEditorButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuMapEditorButton", mapEditorButtonPosition, buttonSize, buttonColor, "Map Editor",
        graphics::Color::Black, 35, fontPath, utils::Vector2f{1, 1}, runMapEditor,
        mapEditorButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(mapEditorButtonConfig));

    const auto controlsButtonOnMouseOver = [=]
    {
        menuState->unselectAllButtons();
        menuState->changeSelectedButton(2);
        menuState->uiManager->setColor(components::ui::UIComponentType::Button, "menuControlsButton",
                                       buttonHoverColor);
    };
    const auto controlsButtonOnMouseOut = [=]
    {
        menuState->unselectAllButtons();
        menuState->hideIcons();
        menuState->uiManager->setColor(components::ui::UIComponentType::Button, "menuControlsButton",
                                       buttonColor);
    };
    auto controlsButtonMouseOverActions =
        components::ui::MouseOverActions{controlsButtonOnMouseOver, controlsButtonOnMouseOut};
    const auto runControls = [=]
    {
        menuState->states.top()->deactivate();
        menuState->states.push(std::make_unique<ControlsState>(
            menuState->window, menuState->inputManager, menuState->rendererPool, menuState->states,
            std::make_unique<components::ui::DefaultUIManager>(menuState->inputManager,
                                                               menuState->rendererPool)));
    };
    auto controlsButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuControlsButton", controlsButtonPosition, buttonSize, buttonColor, "Controls",
        graphics::Color::Black, 35, fontPath, utils::Vector2f{3, 1}, runControls,
        controlsButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(controlsButtonConfig));

    const auto settingsButtonOnMouseOver = [=]
    {
        menuState->unselectAllButtons();
        menuState->changeSelectedButton(3);
        menuState->uiManager->setColor(components::ui::UIComponentType::Button, "menuSettingsButton",
                                       buttonHoverColor);
    };
    const auto settingsButtonOnMouseOut = [=]
    {
        menuState->unselectAllButtons();
        menuState->hideIcons();
        menuState->uiManager->setColor(components::ui::UIComponentType::Button, "menuSettingsButton",
                                       buttonColor);
    };
    auto settingsButtonMouseOverActions =
        components::ui::MouseOverActions{settingsButtonOnMouseOver, settingsButtonOnMouseOut};
    const auto runSettings = [=]
    {
        menuState->states.top()->deactivate();
        menuState->states.push(std::make_unique<SettingsState>(
            menuState->window, menuState->inputManager, menuState->rendererPool, menuState->states,
            std::make_unique<components::ui::DefaultUIManager>(menuState->inputManager,
                                                               menuState->rendererPool)));
    };
    auto settingsButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuSettingsButton", settingsButtonPosition, buttonSize, buttonColor, "Settings",
        graphics::Color::Black, 35, fontPath, utils::Vector2f{3, 1}, runSettings,
        settingsButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(settingsButtonConfig));

    const auto exitButtonOnMouseOver = [=]
    {
        menuState->unselectAllButtons();
        menuState->changeSelectedButton(4);
        menuState->uiManager->setColor(components::ui::UIComponentType::Button, "menuExitButton",
                                       buttonHoverColor);
    };
    const auto exitButtonOnMouseOut = [=]
    {
        menuState->unselectAllButtons();
        menuState->hideIcons();
        menuState->uiManager->setColor(components::ui::UIComponentType::Button, "menuExitButton",
                                       buttonColor);
    };
    auto exitButtonMouseOverActions =
        components::ui::MouseOverActions{exitButtonOnMouseOver, exitButtonOnMouseOut};
    // TODO: to check this window->close if ok, if it isn't set bool variable in menu class to exit game
    const auto exit = [=] { menuState->window->close(); };
    auto exitButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuExitButton", exitButtonPosition, buttonSize, buttonColor, "Exit", graphics::Color::Black, 35,
        fontPath, utils::Vector2f{7, 1}, exit, exitButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(exitButtonConfig));

    return buttonsConfig;
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
MenuStateUIConfigBuilder::createCheckBoxConfigs(MenuState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::LabelConfig>>
MenuStateUIConfigBuilder::createLabelConfigs(MenuState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
MenuStateUIConfigBuilder::createTextFieldConfigs(MenuState*)
{
    return {};
}
}