#include "MainMenuState.h"

namespace game
{

MainMenuState::MainMenuState(const std::shared_ptr<window::Window>& window,
                             const std::shared_ptr<input::InputManager>& inputManager,
                             const std::shared_ptr<graphics::RendererPool>& rendererPool)
    : State{window, inputManager, rendererPool}
{
}
void MainMenuState::initialize() {}
void MainMenuState::update(const utils::DeltaTime&) {}
void MainMenuState::lateUpdate(const utils::DeltaTime&) {}
void MainMenuState::render() {}
}