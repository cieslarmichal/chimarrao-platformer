#include "State.h"

namespace game
{

State::State(std::shared_ptr<window::Window> windowInit,
             std::shared_ptr<input::InputManager> inputManagerInit,
             std::shared_ptr<graphics::RendererPool> rendererPoolInit,
             std::shared_ptr<utils::FileAccess> fileAccessInit,
             std::stack<std::unique_ptr<State>>& statesInit)
    : window{std::move(windowInit)},
      inputManager{std::move(inputManagerInit)},
      rendererPool{std::move(rendererPoolInit)},
      fileAccess{std::move(fileAccessInit)},
      states{statesInit},
      active{false}
{
    window->registerObserver(this);
}

State::~State()
{
    window->removeObserver(this);
}

void State::handleWindowSizeChange(const utils::Vector2u& windowSize)
{
    rendererPool->setRenderingSize(windowSize);
}

}