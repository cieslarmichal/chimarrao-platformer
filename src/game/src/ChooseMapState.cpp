#include "ChooseMapState.h"

#include "ChooseMapStateUIConfigBuilder.h"

namespace game
{
ChooseMapState::ChooseMapState(const std::shared_ptr<window::Window>& windowInit,
                               const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                               std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                               std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      uiManager{std::move(uiManagerInit)},
      mapFilePathsReader{std::make_unique<MapFilePathsReader>(fileAccess)},
      shouldBackToMenu{false}
{
    mapFiles = mapFilePathsReader->readMapFilePaths();
    uiManager->createUI(ChooseMapStateUIConfigBuilder::createChooseMapUIConfig(this));
}

NextState ChooseMapState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void ChooseMapState::lateUpdate(const utils::DeltaTime&) {}

void ChooseMapState::render()
{
    rendererPool->renderAll();
}

StateType ChooseMapState::getType() const
{
    return StateType::ChooseMap;
}

void ChooseMapState::activate()
{
    active = true;
    uiManager->activate();
}

void ChooseMapState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void ChooseMapState::showNextMaps() {}

void ChooseMapState::showPreviousMaps() {}
}