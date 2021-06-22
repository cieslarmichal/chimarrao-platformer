#include "EditorMenuStateUIConfigBuilder.h"

#include "Color.h"
#include "CommonUIConfigElements.h"
#include "EditorMenuState.h"
#include "ProjectPathReader.h"
#include "Vector.h"
#include "nfd.hpp"

namespace
{
std::optional<std::string> getPathToMap();

const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
const auto textColor = graphics::Color(200, 200, 200);
const auto buttonSize = utils::Vector2f{25.f, 5.f};
const auto backToEditorButtonPosition = utils::Vector2f{28.f, 15.f};
const auto loadMapButtonPosition = utils::Vector2f{28.f, 23.f};
const auto newMapButtonPosition = utils::Vector2f{28.f, 31.f};
const auto saveMapButtonPosition = utils::Vector2f{28.f, 39.f};
const auto backToMenuButtonPosition = utils::Vector2f{28.f, 47.f};
const std::vector<utils::Vector2f> iconsPositions{backToEditorButtonPosition, loadMapButtonPosition,
                                                  newMapButtonPosition, saveMapButtonPosition,
                                                  backToMenuButtonPosition};
const auto iconSize = utils::Vector2f{4, 4};
}

namespace game
{
std::vector<std::string> EditorMenuStateUIConfigBuilder::iconNames{
    "editorMenuIcon1Image", "editorMenuIcon2Image", "editorMenuIcon3Image", "editorMenuIcon4Image",
    "editorMenuIcon5Image"};

std::unique_ptr<components::ui::UIConfig>
EditorMenuStateUIConfigBuilder::createEditorMenuUIConfig(EditorMenuState* editorMenuState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(editorMenuState), createButtonConfigs(editorMenuState),
        createCheckBoxConfigs(editorMenuState), createLabelConfigs(editorMenuState),
        createTextFieldConfigs(editorMenuState), createImageConfigs(editorMenuState));
}

std::vector<std::vector<GridButtonInfo>> EditorMenuStateUIConfigBuilder::getGridButtonsInfo()
{
    std::vector<std::vector<GridButtonInfo>> gridButtonsInfo{
        {GridButtonInfo{"editorMenuBackToEditorButton", 0, false, false}},
        {GridButtonInfo{"editorMenuLoadMapButton", 1, false, false}},
        {GridButtonInfo{"editorMenuNewMapButton", 2, false, false}},
        {GridButtonInfo{"editorMenuSaveMapButton", 3, false, false}},
        {GridButtonInfo{"editorMenuBackToMenuButton", 4, false, false}}};

    return gridButtonsInfo;
}

std::vector<std::string> EditorMenuStateUIConfigBuilder::getIconNames()
{
    return iconNames;
}

std::unique_ptr<components::ui::BackgroundConfig>
EditorMenuStateUIConfigBuilder::createBackgroundConfig(EditorMenuState*)
{
    const auto backgroundColor = graphics::Color{172};
    return std::make_unique<components::ui::BackgroundConfig>(
        "editorMenuBackground", utils::Vector2f{22.f, 5.f}, utils::Vector2f{36.f, 50.f},
        graphics::VisibilityLayer::Second, backgroundColor);
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
EditorMenuStateUIConfigBuilder::createButtonConfigs(EditorMenuState* editorMenuState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;

    const auto backToEditorButtonOnMouseOver = [=]
    {
        editorMenuState->buttonsNavigator->setFocusOnButton("editorMenuBackToEditorButton");
        editorMenuState->uiManager->setColor("editorMenuBackToEditorButton", buttonHoverColor);
    };
    const auto backToEditorButtonOnMouseOut = [=]
    {
        editorMenuState->buttonsNavigator->loseFocus();
        editorMenuState->uiManager->setColor("editorMenuBackToEditorButton", buttonColor);
    };
    auto backToEditorButtonMouseOverActions =
        components::ui::MouseOverActions{backToEditorButtonOnMouseOver, backToEditorButtonOnMouseOut};
    auto backToEditorClickAction = [=] { editorMenuState->shouldBackToEditor = true; };
    auto backToEditorButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuBackToEditorButton", backToEditorButtonPosition, buttonSize, buttonColor, "Back to editor",
        textColor, 27, fontPath, utils::Vector2f{1.5f, 0.75f}, backToEditorClickAction,
        backToEditorButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToEditorButtonConfig));

    const auto loadMapButtonOnMouseOver = [=]
    {
        editorMenuState->buttonsNavigator->setFocusOnButton("editorMenuLoadMapButton");
        editorMenuState->uiManager->setColor("editorMenuLoadMapButton", buttonHoverColor);
    };
    const auto loadMapButtonOnMouseOut = [=]
    {
        editorMenuState->buttonsNavigator->loseFocus();
        editorMenuState->uiManager->setColor("editorMenuLoadMapButton", buttonColor);
    };
    auto loadMapButtonMouseOverActions =
        components::ui::MouseOverActions{loadMapButtonOnMouseOver, loadMapButtonOnMouseOut};
    auto loadMapClickAction = [=]
    {
        auto pathToMapFile = getPathToMap();
        if (not pathToMapFile)
        {
            return;
        }
        editorMenuState->tileMap->loadFromFile(*pathToMapFile);
        editorMenuState->shouldBackToEditor = true;
    };
    auto loadMapButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuLoadMapButton", loadMapButtonPosition, buttonSize, buttonColor, "Load map", textColor, 27,
        fontPath, utils::Vector2f{7.f, 0.75f}, loadMapClickAction, loadMapButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(loadMapButtonConfig));

    const auto newMapButtonOnMouseOver = [=]
    {
        editorMenuState->buttonsNavigator->setFocusOnButton("editorMenuNewMapButton");
        editorMenuState->uiManager->setColor("editorMenuNewMapButton", buttonHoverColor);
    };
    const auto newMapButtonOnMouseOut = [=]
    {
        editorMenuState->buttonsNavigator->loseFocus();
        editorMenuState->uiManager->setColor("editorMenuNewMapButton", buttonColor);
    };
    auto newMapButtonMouseOverActions =
        components::ui::MouseOverActions{newMapButtonOnMouseOver, newMapButtonOnMouseOut};
    auto newMapClickAction = [=]
    {
        std::cout << "new map\n";
        TileMapInfo tileMapInfo = TileMapInfo{"", {40, 15}, {}};
        editorMenuState->tileMap->setTileMapInfo(tileMapInfo);
        editorMenuState->shouldBackToEditor = true;
    };
    auto newMapButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuNewMapButton", newMapButtonPosition, buttonSize, buttonColor, "New map", textColor, 27,
        fontPath, utils::Vector2f{7.f, 0.75f}, newMapClickAction, newMapButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(newMapButtonConfig));

    const auto saveMapButtonOnMouseOver = [=]
    {
        editorMenuState->buttonsNavigator->setFocusOnButton("editorMenuSaveMapButton");
        editorMenuState->uiManager->setColor("editorMenuSaveMapButton", buttonHoverColor);
    };
    const auto saveMapButtonOnMouseOut = [=]
    {
        editorMenuState->buttonsNavigator->loseFocus();
        editorMenuState->uiManager->setColor("editorMenuSaveMapButton", buttonColor);
    };
    auto saveMapButtonMouseOverActions =
        components::ui::MouseOverActions{saveMapButtonOnMouseOver, saveMapButtonOnMouseOut};
    auto saveMapClickAction = [=]
    {
        editorMenuState->states.deactivateCurrentState();
        editorMenuState->states.addNextState(StateType::SaveMap);
    };
    auto saveMapButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuSaveMapButton", saveMapButtonPosition, buttonSize, buttonColor, "Save map", textColor, 27,
        fontPath, utils::Vector2f{6.f, 0.75f}, saveMapClickAction, saveMapButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(saveMapButtonConfig));

    const auto backToMenuButtonOnMouseOver = [=]
    {
        editorMenuState->buttonsNavigator->setFocusOnButton("editorMenuBackToMenuButton");
        editorMenuState->uiManager->setColor("editorMenuBackToMenuButton", buttonHoverColor);
    };
    const auto backToMenuButtonOnMouseOut = [=]
    {
        editorMenuState->buttonsNavigator->loseFocus();
        editorMenuState->uiManager->setColor("editorMenuBackToMenuButton", buttonColor);
    };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [=]
    {
        editorMenuState->shouldBackToMenu = true;
        TileMapInfo tileMapInfo = TileMapInfo{"", {40, 15}, {}};
        editorMenuState->tileMap->setTileMapInfo(tileMapInfo);
        editorMenuState->shouldBackToMenu = true;
    };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuBackToMenuButton", backToMenuButtonPosition, buttonSize, buttonColor, "Back to menu",
        textColor, 27, fontPath, utils::Vector2f{2.75f, 0.75f}, backToMenuClickAction,
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

    const auto editorMenuTitlePosition = utils::Vector2f{27.5f, 7.f};
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

std::vector<std::unique_ptr<components::ui::ImageConfig>>
EditorMenuStateUIConfigBuilder::createImageConfigs(EditorMenuState*)
{
    std::vector<std::unique_ptr<components::ui::ImageConfig>> imagesConfig;

    for (std::size_t iconIndex = 0; iconIndex < iconNames.size(); iconIndex++)
    {
        const auto iconPosition = utils::Vector2f{iconsPositions[iconIndex].x - iconSize.x - 0.25f,
                                                  iconsPositions[iconIndex].y + 0.4f};
        auto imageConfig = std::make_unique<components::ui::ImageConfig>(
            iconNames[iconIndex], iconPosition, iconSize, graphics::VisibilityLayer::First, iconPath);
        imagesConfig.push_back(std::move(imageConfig));
    }

    return imagesConfig;
}

}

namespace
{
std::optional<std::string> getPathToMap()
{
    NFD_Init();
    nfdchar_t* outPath;
    auto mapsPath = utils::ProjectPathReader::getProjectRootPath() + "maps";
    nfdfilteritem_t filterItem[1] = {{"Map File", "map"}};
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, mapsPath.c_str());
    if (result == NFD_OKAY)
    {
        auto pathToMap = std::string(outPath);
        std::cout << "Path to map: " << pathToMap << std::endl;
        NFD_FreePath(outPath);
        NFD_Quit();
        return pathToMap;
    }
    else
    {
        NFD_Quit();
        return std::nullopt;
    }
}
}