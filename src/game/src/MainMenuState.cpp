#include "MainMenuState.h"

namespace game
{

MainMenuState::MainMenuState(const std::shared_ptr<window::Window>& window,
                             const std::shared_ptr<input::InputManager>& inputManager,
                             const std::shared_ptr<graphics::RendererPool>& rendererPool, std::stack<std::unique_ptr<State>>& states)
    : State{window, inputManager, rendererPool, states}
{
}

void MainMenuState::initialize() {}
void MainMenuState::update(const utils::DeltaTime&) {}
void MainMenuState::lateUpdate(const utils::DeltaTime&) {}
void MainMenuState::render() {}
}