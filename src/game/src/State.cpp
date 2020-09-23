#include "State.h"

namespace game
{

State::State(std::shared_ptr<gui::Window> windowInit,
                     std::shared_ptr<input::InputManager> inputManagerInit,
                     std::shared_ptr<graphics::RendererPool> rendererPoolInit)
    : window{std::move(windowInit)},
      inputManager{std::move(inputManagerInit)},
      rendererPool{std::move(rendererPoolInit)}
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