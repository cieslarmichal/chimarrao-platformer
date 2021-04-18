#include "State.h"

namespace game
{

State::State(std::shared_ptr<window::Window> windowInit,
             std::shared_ptr<graphics::RendererPool> rendererPoolInit, States& statesInit)
    : window{std::move(windowInit)},
      rendererPool{std::move(rendererPoolInit)},
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