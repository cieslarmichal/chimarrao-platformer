#include "ClickableComponent.h"

#include <utility>

#include "ComponentOwner.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components
{

ClickableComponent::ClickableComponent(ComponentOwner* ownerInit,
                                       std::shared_ptr<input::InputManager> inputManagerInit,
                                       std::function<void(void)> actionInit)
    : Component(ownerInit), inputManager{std::move(inputManagerInit)}, action{std::move(actionInit)}
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
        throw exceptions::DependentComponentNotFound{"Hitbox component not found"};
    }
}

void ClickableComponent::handleInputStatus(const input::InputStatus& inputStatus)
{
    if (inputStatus.isKeyPressed(input::InputKey::MouseLeft) &&
        hitbox->intersects(inputStatus.getMousePosition()))
    {
        action();
    }
}

}