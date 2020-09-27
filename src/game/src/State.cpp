#include "State.h"

namespace game
{

State::State(std::shared_ptr<window::Window> windowInit,
             std::shared_ptr<input::InputManager> inputManagerInit,
             std::shared_ptr<graphics::RendererPool> rendererPoolInit,
             std::stack<std::unique_ptr<State>>& statesInit)
    : window{std::move(windowInit)},
      inputManager{std::move(inputManagerInit)},
      rendererPool{std::move(rendererPoolInit)},
      states{statesInit}
{
    window->registerObserver(this);
}

State::~State()
{
    window->removeObserver(this);
}

void State::windowSizeChanged(const utils::Vector2u& windowSize)
{
    rendererPool->setRenderingSize(windowSize);
}

}