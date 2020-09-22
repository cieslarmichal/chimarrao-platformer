#include "Button.h"

#include <utility>

namespace gui
{

Button::Button(std::shared_ptr<graphics::RendererPool> rendererPool, std::shared_ptr<input::InputManager> inputManagerInit, const utils::Vector2f& size,
               const utils::Vector2f& position)
    : components::ComponentOwner{position}, inputManager{std::move(inputManagerInit)}
{
    graphics = addComponent<components::GraphicsComponent>(std::move(rendererPool), size, position);
    inputManager->registerObserver(this);
}

Button::~Button()
{
    inputManager->removeObserver(this);
}

void Button::handleInputStatus(const input::InputStatus& inputStatus)
{

}

}