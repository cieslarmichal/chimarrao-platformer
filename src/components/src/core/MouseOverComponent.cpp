#include "MouseOverComponent.h"

#include "ComponentOwner.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

MouseOverComponent::MouseOverComponent(ComponentOwner* ownerInit,
                                       std::function<void(void)> mouseOverActionInit,
                                       std::function<void(void)> mouseOutActionInit)
    : Component(ownerInit),
      mouseOverAction{std::move(mouseOverActionInit)},
      mouseOutAction{std::move(mouseOutActionInit)},
      mouseOver{false}
{
}

void MouseOverComponent::loadDependentComponents()
{
    boxCollider = owner->getComponent<BoxColliderComponent>();
    if (not boxCollider)
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

    if (not mouseOver && boxCollider->intersects(input.getMousePosition()))
    {
        mouseOverAction();
        mouseOver = true;
    }
    else if (mouseOver && not boxCollider->intersects(input.getMousePosition()))
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

}