#include "LimitedSpaceActionComponent.h"

#include <cmath>
#include <utility>

#include "ComponentOwner.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{
namespace
{
const auto distance = [](const utils::Vector2f& v1, const utils::Vector2f& v2)
{
    const auto middleOfV1 = utils::Vector2f{v1.x + 2, v1.y + 2};
    const auto middleOfV2 = utils::Vector2f{v2.x + 2, v2.y + 2};
    return std::sqrt(std::pow(middleOfV2.x - middleOfV1.x, 2) + std::pow(middleOfV2.y - middleOfV1.y, 2));
};
}

LimitedSpaceActionComponent::LimitedSpaceActionComponent(ComponentOwner* owner, ComponentOwner* player,
                                                         std::function<void(void)> action,
                                                         double actionThreshold)
    : Component(owner), player{player}, action{std::move(action)}, actionThreshold{actionThreshold}
{
}

void LimitedSpaceActionComponent::loadDependentComponents()
{
    textComponent = owner->getComponent<TextComponent>();
    if (textComponent)
    {
        textComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"LimitedSpaceActionComponent: TextComponent not found"};
    }
    textComponent->disable();
}

void LimitedSpaceActionComponent::update(utils::DeltaTime, const input::Input& input)
{
    if (actionPerformed)
    {
        return;
    }

    const auto ownerPosition = owner->transform->getPosition();
    const auto playerPosition = player->transform->getPosition();
    const auto distanceBetweenOwnerAndPlayer = distance(ownerPosition, playerPosition);

    if (distanceBetweenOwnerAndPlayer < actionThreshold)
    {
        if (not textComponent->isEnabled())
        {
            textComponent->enable();
        }

        if (input.isKeyPressed(input::InputKey::E))
        {
            actionPerformed = true;
            textComponent->disable();
            action();
        }
    }
    else
    {
        if (textComponent->isEnabled())
        {
            textComponent->disable();
        }
    }
}

void LimitedSpaceActionComponent::setAction(std::function<void(void)> newAction)
{
    action = std::move(newAction);
    actionPerformed = false;
}

}