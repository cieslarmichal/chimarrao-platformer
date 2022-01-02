#include "MenuStateUIConfigBuilder.h"

#include "CommonUIConfigElements.h"
#include "MenuState.h"
#include "navigators/PaginatedButtonsNavigator.h"
#include "ProjectPathReader.h"

namespace game
{
namespace
{
const auto newGameButtonPosition = utils::Vector2f{50, 5};
const auto chooseMapButtonPosition = utils::Vector2f{50, 14};
const auto mapEditorButtonPosition = utils::Vector2f{50, 23};
const auto controlsButtonPosition = utils::Vector2f{50, 32};
const auto settingsButtonPosition = utils::Vector2f{50, 41};
const auto exitButtonPosition = utils::Vector2f{50, 50};
const std::vector<utils::Vector2f> buttonsPositions{newGameButtonPosition, chooseMapButtonPosition, mapEditorButtonPosition,
                                                    controlsButtonPosition, settingsButtonPosition,
                                                    exitButtonPosition};
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto buttonSize = utils::Vector2f{23, 6};
const auto iconSize = utils::Vector2f{4, 4};
}

std::vector<std::string> MenuStateUIConfigBuilder::iconNames{
    "menuIcon1Image", "menuIcon2Image", "menuIcon3Image", "menuIcon4Image", "menuIcon5Image", "menuIcon6Image"};

std::unique_ptr<components::ui::UIConfig> MenuStateUIConfigBuilder::createMenuUIConfig(MenuState* menuState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(menuState), createButtonConfigs(menuState), createCheckBoxConfigs(menuState),
        createLabelConfigs(menuState), createTextFieldConfigs(menuState), createImageConfigs(menuState));
}

std::vector<std::vector<GridButtonInfo>> MenuStateUIConfigBuilder::getGridButtonsInfo()
{
    std::vector<std::vector<GridButtonInfo>> gridButtonsInfo{
        {GridButtonInfo{"menuNewGameButton", 0, false, false}},
        {GridButtonInfo{"menuChooseMapButton", 1, false, false}},
        {GridButtonInfo{"menuMapEditorButton", 2, false, false}},
        {GridButtonInfo{"menuControlsButton", 3, false, false}},
        {GridButtonInfo{"menuSettingsButton", 4, false, false}},
        {GridButtonInfo{"menuExitButton", 5, false, false}}};

    return gridButtonsInfo;
}

std::vector<std::string> MenuStateUIConfigBuilder::getIconNames()
{
    return iconNames;
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

    const auto newGameButtonOnMouseOver = [=]
    {
        menuState->buttonsNavigator->setFocusOnButton("menuNewGameButton");
        menuState->uiManager->setColor("menuNewGameButton", buttonHoverColor);
    };
    const auto newGameOnMouseOut = [=]
    {
        menuState->buttonsNavigator->loseFocus();
        menuState->uiManager->setColor("menuNewGameButton", buttonColor);
    };
    auto newGameButtonMouseOverActions =
        components::ui::MouseOverActions{newGameButtonOnMouseOver, newGameOnMouseOut};
    const auto newGameButtonClickAction = [=] { menuState->newGame(); };
    auto newGameButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuNewGameButton", newGameButtonPosition, buttonSize, buttonColor, "New game", graphics::Color::Black, 35,
        fontPath, utils::Vector2f{3, 1}, newGameButtonClickAction, newGameButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(newGameButtonConfig));

    const auto chooseMapButtonOnMouseOver = [=]
    {
        menuState->buttonsNavigator->setFocusOnButton("menuChooseMapButton");
        menuState->uiManager->setColor("menuChooseMapButton", buttonHoverColor);
    };
    const auto chooseMapOnMouseOut = [=]
    {
        menuState->buttonsNavigator->loseFocus();
        menuState->uiManager->setColor("menuChooseMapButton", buttonColor);
    };
    auto chooseMapButtonMouseOverActions =
        components::ui::MouseOverActions{chooseMapButtonOnMouseOver, chooseMapOnMouseOut};
    const auto chooseMapButtonClickAction = [=] { menuState->chooseMap(); };
    auto chooseMapButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuChooseMapButton", chooseMapButtonPosition, buttonSize, buttonColor, "Choose map", graphics::Color::Black, 35,
        fontPath, utils::Vector2f{1, 1}, chooseMapButtonClickAction, chooseMapButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(chooseMapButtonConfig));

    const auto mapEditorButtonOnMouseOver = [=]
    {
        menuState->buttonsNavigator->setFocusOnButton("menuMapEditorButton");
        menuState->uiManager->setColor("menuMapEditorButton", buttonHoverColor);
    };
    const auto mapEditorButtonOnMouseOut = [=]
    {
        menuState->buttonsNavigator->loseFocus();
        menuState->uiManager->setColor("menuMapEditorButton", buttonColor);
    };
    auto mapEditorButtonMouseOverActions =
        components::ui::MouseOverActions{mapEditorButtonOnMouseOver, mapEditorButtonOnMouseOut};
    const auto mapEditorButtonClickAction = [=] { menuState->runMapEditor(); };
    auto mapEditorButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuMapEditorButton", mapEditorButtonPosition, buttonSize, buttonColor, "Map Editor",
        graphics::Color::Black, 35, fontPath, utils::Vector2f{1, 1}, mapEditorButtonClickAction,
        mapEditorButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(mapEditorButtonConfig));

    const auto controlsButtonOnMouseOver = [=]
    {
        menuState->buttonsNavigator->setFocusOnButton("menuControlsButton");
        menuState->uiManager->setColor("menuControlsButton", buttonHoverColor);
    };
    const auto controlsButtonOnMouseOut = [=]
    {
        menuState->buttonsNavigator->loseFocus();
        menuState->uiManager->setColor("menuControlsButton", buttonColor);
    };
    auto controlsButtonMouseOverActions =
        components::ui::MouseOverActions{controlsButtonOnMouseOver, controlsButtonOnMouseOut};
    const auto controlsButtonClickAction = [=] { menuState->runControls(); };
    auto controlsButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuControlsButton", controlsButtonPosition, buttonSize, buttonColor, "Controls",
        graphics::Color::Black, 35, fontPath, utils::Vector2f{3, 1}, controlsButtonClickAction,
        controlsButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(controlsButtonConfig));

    const auto settingsButtonOnMouseOver = [=]
    {
        menuState->buttonsNavigator->setFocusOnButton("menuSettingsButton");
        menuState->uiManager->setColor("menuSettingsButton", buttonHoverColor);
    };
    const auto settingsButtonOnMouseOut = [=]
    {
        menuState->buttonsNavigator->loseFocus();
        menuState->uiManager->setColor("menuSettingsButton", buttonColor);
    };
    auto settingsButtonMouseOverActions =
        components::ui::MouseOverActions{settingsButtonOnMouseOver, settingsButtonOnMouseOut};
    const auto settingsButtonClickAction = [=] { menuState->runSettings(); };
    auto settingsButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuSettingsButton", settingsButtonPosition, buttonSize, buttonColor, "Settings",
        graphics::Color::Black, 35, fontPath, utils::Vector2f{3, 1}, settingsButtonClickAction,
        settingsButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(settingsButtonConfig));

    const auto exitButtonOnMouseOver = [=]
    {
        menuState->buttonsNavigator->setFocusOnButton("menuExitButton");
        menuState->uiManager->setColor("menuExitButton", buttonHoverColor);
    };
    const auto exitButtonOnMouseOut = [=]
    {
        menuState->buttonsNavigator->loseFocus();
        menuState->uiManager->setColor("menuExitButton", buttonColor);
    };
    auto exitButtonMouseOverActions =
        components::ui::MouseOverActions{exitButtonOnMouseOver, exitButtonOnMouseOut};
    const auto exitButtonClickAction = [=] { menuState->exit(); };
    auto exitButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "menuExitButton", exitButtonPosition, buttonSize, buttonColor, "Exit", graphics::Color::Black, 35,
        fontPath, utils::Vector2f{7, 1}, exitButtonClickAction, exitButtonMouseOverActions);
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

std::vector<std::unique_ptr<components::ui::ImageConfig>>
MenuStateUIConfigBuilder::createImageConfigs(MenuState*)
{
    std::vector<std::unique_ptr<components::ui::ImageConfig>> imagesConfig;

    for (std::size_t iconIndex = 0; iconIndex < iconNames.size(); iconIndex++)
    {
        const auto iconPosition =
            utils::Vector2f{buttonsPositions[iconIndex].x + buttonSize.x, buttonsPositions[iconIndex].y + 1};
        auto imageConfig = std::make_unique<components::ui::ImageConfig>(
            iconNames[iconIndex], iconPosition, iconSize, graphics::VisibilityLayer::First, iconPath);
        imagesConfig.push_back(std::move(imageConfig));
    }

    return imagesConfig;
}

}