#include "GameOverState.h"

#include "DefaultUIManager.h"
#include "GameOverStateUIConfigBuilder.h"
#include "TimerFactory.h"

namespace game
{

namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
}

GameOverState::GameOverState(const std::shared_ptr<window::Window>& windowInit,
                             const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                             std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                             std::shared_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      shouldBackToMenu{false},
      uiManager{std::move(uiManagerInit)}
{
    uiManager->createUI(GameOverStateUIConfigBuilder::createGameOverUIConfig(this));
}

NextState GameOverState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void GameOverState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void GameOverState::render()
{
    rendererPool->renderAll();
}

StateType GameOverState::getType() const
{
    return StateType::GameOver;
}

void GameOverState::activate()
{
    active = true;
    uiManager->activate();
}

void GameOverState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void GameOverState::backToMenu()
{
    shouldBackToMenu = true;
}

}