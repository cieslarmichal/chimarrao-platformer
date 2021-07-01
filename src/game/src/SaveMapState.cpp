#include "SaveMapState.h"

#include <utility>

#include "SaveMapStateUIConfigBuilder.h"
#include "TimerFactory.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
}

SaveMapState::SaveMapState(const std::shared_ptr<window::Window>& windowInit,
                           const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                           std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                           std::shared_ptr<components::ui::UIManager> uiManagerInit,
                           std::shared_ptr<TileMap> tileMapInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      shouldBackToEditorMenu{false},
      uiManager{std::move(uiManagerInit)},
      tileMap{std::move(tileMapInit)},
      buttonsNavigator{std::make_unique<GridButtonsNavigator>(
          uiManager, SaveMapStateUIConfigBuilder::getGridButtonsInfo(),
          SaveMapStateUIConfigBuilder::getIconNames(), buttonColor, buttonHoverColor,
          utils::TimerFactory::createTimer(), utils::TimerFactory::createTimer())}
{
    uiManager->createUI(SaveMapStateUIConfigBuilder::createSaveMapUIConfig(this));
    buttonsNavigator->initialize();
    uiManager->setText("saveMapNameTextField", tileMap->getName());
}

NextState SaveMapState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (const auto nextState = buttonsNavigator->update(deltaTime, input); nextState == NextState::Previous)
    {
        return NextState::Previous;
    }

    if (shouldBackToEditorMenu)
    {
        return NextState::Previous;
    }

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void SaveMapState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void SaveMapState::render()
{
    rendererPool->renderAll();
}

StateType SaveMapState::getType() const
{
    return StateType::SaveMap;
}

void SaveMapState::activate()
{
    active = true;
    buttonsNavigator->activate();
    uiManager->activate();
}

void SaveMapState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void SaveMapState::backToEditorMenu()
{
    shouldBackToEditorMenu = true;
}

void SaveMapState::saveMap()
{
    tileMap->setName(currentMapName);
    tileMap->saveToFile();
    shouldBackToEditorMenu = true;
}
}