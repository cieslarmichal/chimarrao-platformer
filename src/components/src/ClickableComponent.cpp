#include "ClickableComponent.h"

#include <utility>

#include "ComponentOwner.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components
{

ClickableComponent::ClickableComponent(ComponentOwner* ownerInit,
                                       std::shared_ptr<input::InputManager> inputManagerInit,
                                       std::function<void(void)> actionInit)
    : Component(ownerInit),
      inputManager{std::move(inputManagerInit)},
      inputStatus{nullptr},
      action{std::move(actionInit)},
      clicked{false}
{
    inputManager->registerObserver(this);
}

ClickableComponent::~ClickableComponent()
{
    inputManager->removeObserver(this);
}

void ClickableComponent::loadDependentComponents()
{
    hitbox = owner->getComponent<HitboxComponent>();
    if (not hitbox)
    {
        throw exceptions::DependentComponentNotFound{"ClickableComponent: Hitbox component not found"};
    }
}

void ClickableComponent::update(utils::DeltaTime)
{
    if (not enabled)
    {
        return;
    }

    if (not clicked && inputStatus->isKeyPressed(input::InputKey::MouseLeft) &&
        hitbox->intersects(inputStatus->getMousePosition()))
    {
        action();
        clicked = true;
    }
}

void ClickableComponent::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void ClickableComponent::enable()
{
    // TODO: test
    Component::enable();
    clicked = false;
}

void ClickableComponent::disable()
{
    Component::disable();
    clicked = false;
}

}