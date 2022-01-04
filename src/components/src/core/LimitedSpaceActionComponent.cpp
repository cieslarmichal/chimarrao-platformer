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
{ return std::sqrt(std::pow(v2.x - v1.x, 2) + std::pow(v2.y - v1.y, 2)); };
}

LimitedSpaceActionComponent::LimitedSpaceActionComponent(ComponentOwner* owner, ComponentOwner* player,
                                                         std::function<void(void)> action)
    : Component(owner), player{player}, action{std::move(action)}
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

    playersItemCollector = player->getComponent<ItemCollectorComponent>();
    if (playersItemCollector)
    {
        playersItemCollector->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "LimitedSpaceActionComponent: player's ItemCollectorComponent not found"};
    }
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

    if (distanceBetweenOwnerAndPlayer < minimumDistanceInWhichActionCanBePerformed)
    {
        if (not textComponent->isEnabled())
        {
            textComponent->enable();
        }

        if (playersItemCollector->isEnabled())
        {
            playersItemCollector->disable();
        }

        if (input.isKeyPressed(input::InputKey::E))
        {
            actionPerformed = true;
            textComponent->disable();
            playersItemCollector->enable();
            action();
        }
    }
}

}