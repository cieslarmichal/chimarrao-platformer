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
    hitBox = owner->getComponent<HitBoxComponent>();
    if (not hitBox)
    {
        throw exceptions::DependentComponentNotFound{"MouseOverComponent: HitBox component not found"};
    }
}

void MouseOverComponent::update(utils::DeltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    if (not mouseOver && hitBox->intersects(input.getMousePosition()))
    {
        mouseOverAction();
        mouseOver = true;
    }
    else if (mouseOver && not hitBox->intersects(input.getMousePosition()))
    {
        mouseOutAction();
        mouseOver = false;
    }
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