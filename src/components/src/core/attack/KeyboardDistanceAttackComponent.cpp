#include "KeyboardDistanceAttackComponent.h"

#include "DistanceAttack.h"
#include "TimerFactory.h"

namespace components::core
{

KeyboardDistanceAttackComponent::KeyboardDistanceAttackComponent(
    ComponentOwner* owner, std::shared_ptr<DistanceAttack> distanceAttackInit)
    : Component{owner}, distanceAttack{std::move(distanceAttackInit)}, timeBetweenAttacks{1.f}
{
    attackIntervalTimer = utils::TimerFactory::createTimer();
}

void KeyboardDistanceAttackComponent::update(utils::DeltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    if (attackIntervalTimer->getElapsedSeconds() > timeBetweenAttacks and
        input.isKeyPressed(input::InputKey::F))
    {
        distanceAttack->attack();
        attackIntervalTimer->restart();
    }
}

}