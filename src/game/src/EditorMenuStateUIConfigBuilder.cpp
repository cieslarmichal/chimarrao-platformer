#include "EditorMenuStateUIConfigBuilder.h"

#include "Color.h"
#include "CommonUIConfigElements.h"
#include "EditorMenuState.h"
#include "GetProjectPath.h"
#include "SaveMapState.h"
#include "Vector.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
const auto textColor = graphics::Color(200, 200, 200);
const auto buttonSize = utils::Vector2f{25, 5};
}

std::unique_ptr<components::ui::UIConfig>
EditorMenuStateUIConfigBuilder::createEditorMenuUIConfig(EditorMenuState* editorMenuState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(editorMenuState), std::move(createButtonConfigs(editorMenuState)),
        createCheckBoxConfigs(editorMenuState), createLabelConfigs(editorMenuState),
        createTextFieldConfigs(editorMenuState));
}

std::unique_ptr<components::ui::BackgroundConfig>
EditorMenuStateUIConfigBuilder::createBackgroundConfig(EditorMenuState*)
{
    const auto backgroundColor = graphics::Color{172};
    return std::make_unique<components::ui::BackgroundConfig>(
        "editorMenuBackground", utils::Vector2f{25, 5}, utils::Vector2f{31, 50},
        graphics::VisibilityLayer::Second, backgroundColor);
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
EditorMenuStateUIConfigBuilder::createButtonConfigs(EditorMenuState* editorMenuState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;

    const auto backToEditorButtonPosition = utils::Vector2f{28, 15};
    const auto backToEditorButtonOnMouseOver = [=]
    {
        editorMenuState->uiManager->setColor(components::ui::UIComponentType::Button,
                                             "editorMenuBackToEditorButton", buttonHoverColor);
    };
    const auto backToEditorButtonOnMouseOut = [=]
    {
        editorMenuState->uiManager->setColor(components::ui::UIComponentType::Button,
                                             "editorMenuBackToEditorButton", buttonColor);
    };
    auto backToEditorButtonMouseOverActions =
        components::ui::MouseOverActions{backToEditorButtonOnMouseOver, backToEditorButtonOnMouseOut};
    auto backToEditorClickAction = [=] { editorMenuState->shouldBackToEditor = true; };
    auto backToEditorButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuBackToEditorButton", backToEditorButtonPosition, buttonSize, buttonColor, "Back to editor",
        textColor, 27, fontPath, utils::Vector2f{1.5, 0.75}, backToEditorClickAction,
        backToEditorButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToEditorButtonConfig));

    const auto loadMapButtonPosition = utils::Vector2f{28, 23};
    const auto loadMapButtonOnMouseOver = [=]
    {
        editorMenuState->uiManager->setColor(components::ui::UIComponentType::Button,
                                             "editorMenuLoadMapButton", buttonHoverColor);
    };
    const auto loadMapButtonOnMouseOut = [=]
    {
        editorMenuState->uiManager->setColor(components::ui::UIComponentType::Button,
                                             "editorMenuLoadMapButton", buttonColor);
    };
    auto loadMapButtonMouseOverActions =
        components::ui::MouseOverActions{loadMapButtonOnMouseOver, loadMapButtonOnMouseOut};
    auto loadMapClickAction = [&] { std::cout << "load map\n"; };
    auto loadMapButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuLoadMapButton", loadMapButtonPosition, buttonSize, buttonColor, "Load map", textColor, 27,
        fontPath, utils::Vector2f{7, 0.75}, loadMapClickAction, loadMapButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(loadMapButtonConfig));

    const auto newMapButtonPosition = utils::Vector2f{28, 31};
    const auto newMapButtonOnMouseOver = [=]
    {
        editorMenuState->uiManager->setColor(components::ui::UIComponentType::Button,
                                             "editorMenuNewMapButton", buttonHoverColor);
    };
    const auto newMapButtonOnMouseOut = [=]
    {
        editorMenuState->uiManager->setColor(components::ui::UIComponentType::Button,
                                             "editorMenuNewMapButton", buttonColor);
    };
    auto newMapButtonMouseOverActions =
        components::ui::MouseOverActions{newMapButtonOnMouseOver, newMapButtonOnMouseOut};
    auto newMapClickAction = [&] { std::cout << "new map\n"; };
    auto newMapButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuNewMapButton", newMapButtonPosition, buttonSize, buttonColor, "New map", textColor, 27,
        fontPath, utils::Vector2f{7, 0.75}, newMapClickAction, newMapButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(newMapButtonConfig));

    const auto saveMapButtonPosition = utils::Vector2f{28, 39};
    const auto saveMapButtonOnMouseOver = [=]
    {
        editorMenuState->uiManager->setColor(components::ui::UIComponentType::Button,
                                             "editorMenuSaveMapButton", buttonHoverColor);
    };
    const auto saveMapButtonOnMouseOut = [=]
    {
        editorMenuState->uiManager->setColor(components::ui::UIComponentType::Button,
                                             "editorMenuSaveMapButton", buttonColor);
    };
    auto saveMapButtonMouseOverActions =
        components::ui::MouseOverActions{saveMapButtonOnMouseOver, saveMapButtonOnMouseOut};
    auto saveMapClickAction = [=]
    {
        editorMenuState->states.top()->deactivate();
        editorMenuState->states.push(
            std::make_unique<SaveMapState>(editorMenuState->window, editorMenuState->inputManager,
                                           editorMenuState->rendererPool, editorMenuState->states,
                                           std::make_unique<components::ui::DefaultUIManager>(
                                               editorMenuState->inputManager, editorMenuState->rendererPool),
                                           editorMenuState->tileMap));
    };
    auto saveMapButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuSaveMapButton", saveMapButtonPosition, buttonSize, buttonColor, "Save map", textColor, 27,
        fontPath, utils::Vector2f{6, 0.75}, saveMapClickAction, saveMapButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(saveMapButtonConfig));

    const auto backToMenuButtonPosition = utils::Vector2f{28, 47};
    const auto backToMenuButtonOnMouseOver = [=]
    {
        editorMenuState->uiManager->setColor(components::ui::UIComponentType::Button,
                                             "editorMenuBackToMenuButton", buttonHoverColor);
    };
    const auto backToMenuButtonOnMouseOut = [=]
    {
        editorMenuState->uiManager->setColor(components::ui::UIComponentType::Button,
                                             "editorMenuBackToMenuButton", buttonColor);
    };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [=] { editorMenuState->shouldBackToMenu = true; };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuBackToMenuButton", backToMenuButtonPosition, buttonSize, buttonColor, "Back to menu",
        textColor, 27, fontPath, utils::Vector2f{2.75, 0.75}, backToMenuClickAction,
        backToMenuButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToMenuButtonConfig));

    return buttonsConfig;
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
EditorMenuStateUIConfigBuilder::createCheckBoxConfigs(EditorMenuState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::LabelConfig>>
EditorMenuStateUIConfigBuilder::createLabelConfigs(EditorMenuState*)
{
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;

    const auto editorMenuTitlePosition = utils::Vector2f{27.5, 7};
    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "editorMenuTitleLabel", editorMenuTitlePosition, graphics::Color::White, "Editor Menu", 40, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

    return labelsConfig;
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
EditorMenuStateUIConfigBuilder::createTextFieldConfigs(EditorMenuState*)
{
    return {};
}
}