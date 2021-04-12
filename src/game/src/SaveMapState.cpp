#include "SaveMapState.h"

#include "SaveMapStateUIConfigBuilder.h"

namespace game
{

SaveMapState::SaveMapState(const std::shared_ptr<window::Window>& windowInit,
                           const std::shared_ptr<input::InputManager>& inputManagerInit,
                           const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                           std::stack<std::unique_ptr<State>>& statesInit,
                           std::unique_ptr<components::ui::UIManager> uiManagerInit, TileMap& tileMap)
    : State{windowInit, inputManagerInit, rendererPoolInit, statesInit},
      inputStatus{nullptr},
      timeAfterLeaveStateIsPossible{0.5f},
      shouldBackToEditorMenu{false},
      uiManager{std::move(uiManagerInit)},
      tileMap{tileMap}
{
    inputManager->registerObserver(this);
    uiManager->createUI(SaveMapStateUIConfigBuilder::createSaveMapUIConfig(this));

    possibleLeaveFromStateTimer.start();
}

SaveMapState::~SaveMapState()
{
    inputManager->removeObserver(this);
}

NextState SaveMapState::update(const utils::DeltaTime& deltaTime)
{
    if (possibleLeaveFromStateTimer.getElapsedSeconds() > timeAfterLeaveStateIsPossible &&
        (inputStatus->isKeyPressed(input::InputKey::Escape) || shouldBackToEditorMenu))
    {
        return NextState::Previous;
    }

    uiManager->update(deltaTime);
    return NextState::Same;
}

void SaveMapState::lateUpdate(const utils::DeltaTime& deltaTime) {}

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

void SaveMapState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
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