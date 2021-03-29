#include "ClickableComponent.h"

#include <unordered_set>
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
    keyActions.push_back({input::InputKey::MouseLeft, std::move(actionInit)});
    inputManager->registerObserver(this);
}

ClickableComponent::ClickableComponent(ComponentOwner* ownerInit,
                                       std::shared_ptr<input::InputManager> inputManagerInit,
                                       const std::vector<KeyAction>& keyActionVectorInit)
    : Component(ownerInit), inputManager{std::move(inputManagerInit)}, inputStatus{nullptr}
{
    inputManager->registerObserver(this);

    std::unordered_set<input::InputKey> inputKeys;
    for (const auto& keyAction : keyActionVectorInit)
    {
        if (inputKeys.count(keyAction.key))
        {
            throw exceptions::ActionForKeyAlreadyExists{
                "ClickableComponent: Two or more action for the same key"};
        }
        inputKeys.insert(keyAction.key);
        keyActions.push_back(keyAction);
    }
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

    for (auto& keyAction : keyActions)
    {
        if (not keyAction.clicked && inputStatus->isKeyReleased(keyAction.key) &&
            hitbox->intersects(inputStatus->getMousePosition()))
        {
            keyAction.action();
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
    for (auto& keyAction : keyActions)
    {
        keyAction.clicked = false;
    }
}

void ClickableComponent::disable()
{
    Component::disable();
    for (auto& keyAction : keyActions)
    {
        keyAction.clicked = false;
    }
}
}