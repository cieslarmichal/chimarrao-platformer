#include "MouseOverComponent.h"

#include "ComponentOwner.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

MouseOverComponent::MouseOverComponent(ComponentOwner* ownerInit,
                                       std::function<void(void)> mouseOverActionInit,
                                       std::function<void(void)> mouseOutActionInit, bool relative)
    : Component(ownerInit),
      mouseOverAction{std::move(mouseOverActionInit)},
      mouseOutAction{std::move(mouseOutActionInit)},
      mouseOver{false},
      relative{relative}
{
}

void MouseOverComponent::loadDependentComponents()
{
    boxCollider = owner->getComponent<BoxColliderComponent>();
    if (boxCollider)
    {
        boxCollider->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"MouseOverComponent: BoxColliderComponent not found"};
    }
}

void MouseOverComponent::update(utils::DeltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    const auto mousePosition = relative ? input.getMouseRelativePosition() : input.getMouseAbsolutePosition();

    if (not mouseOver && boxCollider->intersects(mousePosition))
    {
        mouseOverAction();
        mouseOver = true;
    }
    else if (mouseOver && not boxCollider->intersects(mousePosition))
    {
        mouseOutAction();
        mouseOver = false;
    }
}

void MouseOverComponent::enable()
{
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

void MouseOverComponent::invokeMouseOverAction()
{
    mouseOverAction();
}

void MouseOverComponent::invokeMouseOutAction()
{
    mouseOutAction();
}

}