#pragma once

#include "ComponentOwner.h"
#include "Timer.h"

namespace components::core
{
class DistanceAttack;

class KeyboardDistanceAttackComponent : public Component
{
public:
    KeyboardDistanceAttackComponent(ComponentOwner* owner, std::shared_ptr<DistanceAttack>);

    void update(utils::DeltaTime, const input::Input&) override;

private:
    std::shared_ptr<DistanceAttack> distanceAttack;
    std::unique_ptr<utils::Timer> attackIntervalTimer;
    const float timeBetweenAttacks;
};
}
