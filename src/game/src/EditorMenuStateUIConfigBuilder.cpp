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
}

namespace game
{
std::unique_ptr<components::ui::UIConfig>
EditorMenuStateUIConfigBuilder::createEditorMenuUIConfig(EditorMenuState* editorMenuState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(editorMenuState), createButtonConfigs(editorMenuState),
        createCheckBoxConfigs(editorMenuState), createLabelConfigs(editorMenuState),
        createTextFieldConfigs(editorMenuState), createImageConfigs(editorMenuState));
}

std::unique_ptr<components::ui::BackgroundConfig>
EditorMenuStateUIConfigBuilder::createBackgroundConfig(EditorMenuState*)
{
    const auto backgroundColor = graphics::Color{172};
    return std::make_unique<components::ui::BackgroundConfig>(
        "editorMenuBackground", utils::Vector2f{25.f, 5.f}, utils::Vector2f{31.f, 50.f},
        graphics::VisibilityLayer::Second, backgroundColor);
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
EditorMenuStateUIConfigBuilder::createButtonConfigs(EditorMenuState* editorMenuState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;

    const auto backToEditorButtonPosition = utils::Vector2f{28.f, 15.f};
    const auto backToEditorButtonOnMouseOver = [=]
    { editorMenuState->uiManager->setColor("editorMenuBackToEditorButton", buttonHoverColor); };
    const auto backToEditorButtonOnMouseOut = [=]
    { editorMenuState->uiManager->setColor("editorMenuBackToEditorButton", buttonColor); };
    auto backToEditorButtonMouseOverActions =
        components::ui::MouseOverActions{backToEditorButtonOnMouseOver, backToEditorButtonOnMouseOut};
    auto backToEditorClickAction = [=] { editorMenuState->shouldBackToEditor = true; };
    auto backToEditorButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "editorMenuBackToEditorButton", backToEditorButtonPosition, buttonSize, buttonColor, "Back to editor",
        textColor, 27, fontPath, utils::Vector2f{1.5f, 0.75f}, backToEditorClickAction,
        backToEditorButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToEditorButtonConfig));

    const auto loadMapButtonPosition = utils::Vector2f{28.f, 23.f};
    const auto loadMapButtonOnMouseOver = [=]
    { editorMenuState->uiManager->setColor("editorMenuLoadMapButton", buttonHoverColor); };
    const auto loadMapButtonOnMouseOut = [=]
    { editorMenuState->uiManager->setColor("editorMenuLoadMapButton", buttonColor); };
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

    const auto newMapButtonPosition = utils::Vector2f{28.f, 31.f};
    const auto newMapButtonOnMouseOver = [=]
    { editorMenuState->uiManager->setColor("editorMenuNewMapButton", buttonHoverColor); };
    const auto newMapButtonOnMouseOut = [=]
    { editorMenuState->uiManager->setColor("editorMenuNewMapButton", buttonColor); };
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

    const auto saveMapButtonPosition = utils::Vector2f{28.f, 39.f};
    const auto saveMapButtonOnMouseOver = [=]
    { editorMenuState->uiManager->setColor("editorMenuSaveMapButton", buttonHoverColor); };
    const auto saveMapButtonOnMouseOut = [=]
    { editorMenuState->uiManager->setColor("editorMenuSaveMapButton", buttonColor); };
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

    const auto backToMenuButtonPosition = utils::Vector2f{28.f, 47.f};
    const auto backToMenuButtonOnMouseOver = [=]
    { editorMenuState->uiManager->setColor("editorMenuBackToMenuButton", buttonHoverColor); };
    const auto backToMenuButtonOnMouseOut = [=]
    { editorMenuState->uiManager->setColor("editorMenuBackToMenuButton", buttonColor); };
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
    return {};
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