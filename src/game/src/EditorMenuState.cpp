#include "EditorMenuState.h"

#include <utility>

#include "EditorMenuStateUIConfigBuilder.h"
#include "TimerFactory.h"
#include "nfd.hpp"

namespace game
{
namespace
{
std::optional<std::string> getPathToMap();

const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
}

EditorMenuState::EditorMenuState(const std::shared_ptr<window::Window>& windowInit,
                                 const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                                 std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                                 std::shared_ptr<components::ui::UIManager> uiManagerInit,
                                 std::shared_ptr<TileMap> tileMapInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      shouldBackToEditor{false},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)},
      tileMap{std::move(tileMapInit)},
      buttonsNavigator{std::make_unique<GridButtonsNavigator>(
          uiManager, EditorMenuStateUIConfigBuilder::getGridButtonsInfo(),
          EditorMenuStateUIConfigBuilder::getIconNames(), buttonColor, buttonHoverColor,
          utils::TimerFactory::createTimer(), utils::TimerFactory::createTimer())}
{
    uiManager->createUI(EditorMenuStateUIConfigBuilder::createEditorMenuUIConfig(this));
    buttonsNavigator->initialize();
}

NextState EditorMenuState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (const auto nextState = buttonsNavigator->update(deltaTime, input); nextState == NextState::Previous)
    {
        return NextState::Previous;
    }

    if (shouldBackToEditor)
    {
        return NextState::Previous;
    }

    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void EditorMenuState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void EditorMenuState::render()
{
    rendererPool->renderAll();
}

StateType EditorMenuState::getType() const
{
    return StateType::EditorMenu;
}

void EditorMenuState::activate()
{
    active = true;
    uiManager->activate();
    buttonsNavigator->activate();
}

void EditorMenuState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void EditorMenuState::backToEditor()
{
    shouldBackToEditor = true;
}

void EditorMenuState::backToMenu()
{
    const auto tileMapInfo = TileMapInfo{"", {40, 15}, {}};
    tileMap->setTileMapInfo(tileMapInfo);
    shouldBackToMenu = true;
}

void EditorMenuState::saveMap()
{
    states.deactivateCurrentState();
    states.addNextState(StateType::SaveMap);
}

void EditorMenuState::createNewMap()
{
    const auto tileMapInfo = TileMapInfo{"", {40, 15}, {}};
    tileMap->setTileMapInfo(tileMapInfo);
    shouldBackToEditor = true;
}

void EditorMenuState::loadMap()
{
    if (const auto pathToMapFile = getPathToMap())
    {
        tileMap->loadFromFile(*pathToMapFile);
        shouldBackToEditor = true;
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
}