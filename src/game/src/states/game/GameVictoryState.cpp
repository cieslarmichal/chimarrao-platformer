#include "GameVictoryState.h"

#include "GameVictoryStateUIConfigBuilder.h"
#include "TimerFactory.h"

namespace game
{

namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
}

GameVictoryState::GameVictoryState(const std::shared_ptr<window::Window>& windowInit,
                                   const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                                   std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                                   std::shared_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)}
{
    uiManager->createUI(GameVictoryStateUIConfigBuilder::createGameVictoryUIConfig(this));
}

NextState GameVictoryState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void GameVictoryState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void GameVictoryState::render()
{
    rendererPool->renderAll();
}

StateType GameVictoryState::getType() const
{
    return StateType::GameVictory;
}

void GameVictoryState::activate()
{
    active = true;
    uiManager->activate();
}

void GameVictoryState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void GameVictoryState::backToMenu()
{
    shouldBackToMenu = true;
}

}