#include "SaveMapState.h"

#include <utility>

#include "SaveMapStateUIConfigBuilder.h"

namespace game
{

SaveMapState::SaveMapState(const std::shared_ptr<window::Window>& windowInit,
                           const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                           std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                           std::unique_ptr<components::ui::UIManager> uiManagerInit,
                           std::shared_ptr<TileMap> tileMapInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToEditorMenu{false},
      uiManager{std::move(uiManagerInit)},
      tileMap{tileMapInit}
{
    uiManager->createUI(SaveMapStateUIConfigBuilder::createSaveMapUIConfig(this));

    possibleLeaveFromStateTimer.start();
}

NextState SaveMapState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (possibleLeaveFromStateTimer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        (input.isKeyPressed(input::InputKey::Escape) || shouldBackToEditorMenu))
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

    possibleLeaveFromStateTimer.restart();
    uiManager->activate();
}

void SaveMapState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void SaveMapState::saveMap()
{
    tileMap->setName(currentMapName);
    tileMap->saveToFile();
    std::cout << tileMap->getName() << std::endl;
    std::cout << tileMap->getPath() << std::endl;
    shouldBackToEditorMenu = true;
}

}