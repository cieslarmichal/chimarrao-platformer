#include "ClickableComponent.h"

#include <unordered_set>
#include <utility>

#include "ComponentOwner.h"
#include "exceptions/ActionForKeyAlreadyExists.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{
ClickableComponent::ClickableComponent(ComponentOwner* ownerInit, std::function<void(void)> actionInit,
                                       bool relative)
    : Component(ownerInit), relative{relative}
{
    keyActions.push_back({input::InputKey::MouseLeft, std::move(actionInit)});
}

ClickableComponent::ClickableComponent(ComponentOwner* ownerInit,
                                       const std::vector<KeyAction>& keyActionsInit, bool relative)
    : Component(ownerInit), relative{relative}
{
    setKeyActions(keyActionsInit);
}

void ClickableComponent::loadDependentComponents()
{
    boxCollider = owner->getComponent<BoxColliderComponent>();
    if (boxCollider)
    {
        boxCollider->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"ClickableComponent: BoxColliderComponent not found"};
    }
}

void ClickableComponent::update(utils::DeltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    for (auto& keyAction : keyActions)
    {
        const auto mousePosition = relative? input.getMouseRelativePosition(): input.getMouseAbsolutePosition();

        if (not keyAction.clicked && input.isKeyReleased(keyAction.key) &&
            boxCollider->intersects(mousePosition))
        {
            keyAction.action();
        }
    }
}

void ClickableComponent::enable()
{
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

void ClickableComponent::setKeyActions(const std::vector<KeyAction>& keyActionsInit)
{
    keyActions.clear();

    std::unordered_set<input::InputKey> inputKeys;
    for (const auto& keyAction : keyActionsInit)
    {
        if (inputKeys.contains(keyAction.key))
        {
            throw exceptions::ActionForKeyAlreadyExists{
                "ClickableComponent: Two or more action for the same key"};
        }
        inputKeys.insert(keyAction.key);
        keyActions.push_back(keyAction);
    }
}

void ClickableComponent::invokeKeyAction(input::InputKey inputKey)
{
    auto foundKeyAction = std::find_if(keyActions.begin(), keyActions.end(),
                                       [&](const KeyAction& keyAction) { return keyAction.key == inputKey; });
    if (foundKeyAction != keyActions.end())
    {
        foundKeyAction->action();
    }
}
}