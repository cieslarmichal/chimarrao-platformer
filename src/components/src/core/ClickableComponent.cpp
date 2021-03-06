#include "ClickableComponent.h"

#include <set>
#include <utility>

#include "ComponentOwner.h"
#include "exceptions/ActionForKeyAlreadyExists.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{
ClickableComponent::ClickableComponent(ComponentOwner* ownerInit,
                                       std::shared_ptr<input::InputManager> inputManagerInit,
                                       std::function<void(void)> actionInit)
    : Component(ownerInit), inputManager{std::move(inputManagerInit)}, inputStatus{nullptr}
{
    keyActionVector.push_back({input::InputKey::MouseLeft, std::move(actionInit)});
    inputManager->registerObserver(this);
}

ClickableComponent::ClickableComponent(ComponentOwner* ownerInit,
                                       std::shared_ptr<input::InputManager> inputManagerInit,
                                       std::vector<KeyAction> keyActionVectorInit)
    : Component(ownerInit), inputManager{std::move(inputManagerInit)}, inputStatus{nullptr}
{
    std::set<input::InputKey> inputKeys;
    for (auto& keyAction : keyActionVectorInit)
    {
        if (inputKeys.count(keyAction.key))
        {
            throw exceptions::ActionForKeyAlreadyExists{
                "ClickableComponent: Two or more action for the same key"};
        }
        inputKeys.insert(keyAction.key);
        keyActionVector.push_back(std::move(keyAction));
    }

    inputManager->registerObserver(this);
}

ClickableComponent::~ClickableComponent()
{
    inputManager->removeObserver(this);
}

void ClickableComponent::loadDependentComponents()
{
    hitbox = owner->getComponent<HitBoxComponent>();
    if (not hitbox)
    {
        throw exceptions::DependentComponentNotFound{"ClickableComponent: HitBox component not found"};
    }
}

void ClickableComponent::update(utils::DeltaTime)
{
    if (not enabled)
    {
        return;
    }

    for (auto& keyAction : keyActionVector)
    {
        if (not keyAction.clicked && inputStatus->isKeyReleased(keyAction.key) &&
            hitbox->intersects(inputStatus->getMousePosition()))
        {
            keyAction.action();
            // clicked = true;
        }
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
    for (auto& keyAction : keyActionVector)
    {
        keyAction.clicked = false;
    }
}

void ClickableComponent::disable()
{
    Component::disable();
    for (auto& keyAction : keyActionVector)
    {
        keyAction.clicked = false;
    }
}
}