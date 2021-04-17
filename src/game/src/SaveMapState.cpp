#include "SaveMapState.h"

#include "SaveMapStateUIConfigBuilder.h"

namespace game
{

SaveMapState::SaveMapState(const std::shared_ptr<window::Window>& windowInit,
                           const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                           std::stack<std::unique_ptr<State>>& statesInit,
                           std::unique_ptr<components::ui::UIManager> uiManagerInit, TileMap& tileMap)
    : State{windowInit, rendererPoolInit, statesInit},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToEditorMenu{false},
      uiManager{std::move(uiManagerInit)},
      tileMap{tileMap}
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

void SaveMapState::lateUpdate(const utils::DeltaTime&) {}

void SaveMapState::render()
{
    rendererPool->renderAll();
}

std::string SaveMapState::getName() const
{
    return "Save map state";
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
    tileMap.setName(currentMapName);
    // tileMap.saveToFile();
    std::cout << tileMap.getName() << std::endl;
    std::cout << tileMap.getPath() << std::endl;
    shouldBackToEditorMenu = true;
}

}