#include "KeyboardDistanceAttackComponent.h"

#include "DistanceAttack.h"

namespace components::core
{

KeyboardDistanceAttackComponent::KeyboardDistanceAttackComponent(
    ComponentOwner* owner, std::shared_ptr<DistanceAttack> distanceAttackInit)
    : Component{owner}, distanceAttack{std::move(distanceAttackInit)}

{
}

void KeyboardDistanceAttackComponent::update(utils::DeltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    if (input.isKeyPressed(input::InputKey::Space))
    {
        distanceAttack->attack();
    }
}

}