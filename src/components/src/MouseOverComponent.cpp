#include "MouseOverComponent.h"

#include "ComponentOwner.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components
{

MouseOverComponent::MouseOverComponent(ComponentOwner* ownerInit,
                                       std::shared_ptr<input::InputManager> inputManagerInit,
                                       std::function<void(void)> mouseOverActionInit,
                                       std::function<void(void)> mouseOutActionInit)
    : Component(ownerInit),
      inputManager{std::move(inputManagerInit)},
      inputStatus{nullptr},
      mouseOverAction{std::move(mouseOverActionInit)},
      mouseOutAction{std::move(mouseOutActionInit)},
      mouseOver{false}
{
    inputManager->registerObserver(this);
}

MouseOverComponent::~MouseOverComponent()
{
    inputManager->removeObserver(this);
}

void MouseOverComponent::loadDependentComponents()
{
    hitbox = owner->getComponent<HitboxComponent>();
    if (not hitbox)
    {
        throw exceptions::DependentComponentNotFound{"MouseOverComponent: Hitbox component not found"};
    }
}

void MouseOverComponent::update(utils::DeltaTime)
{
    if (not enabled)
    {
        return;
    }

    if (not mouseOver && hitbox->intersects(inputStatus->getMousePosition()))
    {
        mouseOverAction();
        mouseOver = true;
    }
    else if (mouseOver && not hitbox->intersects(inputStatus->getMousePosition()))
    {
        mouseOutAction();
        mouseOver = false;
    }
}

void MouseOverComponent::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void MouseOverComponent::enable()
{
    // TODO: test
    Component::enable();
    mouseOver = false;
}

void MouseOverComponent::disable()
{
    Component::disable();
    if (mouseOver)
    {
        mouseOutAction();
        mouseOver = false;
    }
}

}