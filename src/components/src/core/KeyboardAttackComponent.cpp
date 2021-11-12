#include "KeyboardAttackComponent.h"

#include "HealthComponent.h"
#include "core/exceptions/DependentComponentNotFound.h"

namespace components::core
{

KeyboardAttackComponent::KeyboardAttackComponent(ComponentOwner* owner,
                                                 std::unique_ptr<AttackStrategy> attackStrategy)
    : Component{owner}, attackStrategy{std::move(attackStrategy)}

{
}

void KeyboardAttackComponent::loadDependentComponents()
{
    animation = owner->getComponent<AnimationComponent>();
    if (animation)
    {
        animation->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"MeleeAttackComponent: Animation component not found"};
    }
}

void KeyboardAttackComponent::update(utils::DeltaTime, const input::Input& input)
{
    if (input.isKeyPressed(input::InputKey::Space) and
        animation->getAnimationType() != animations::AnimationType::Attack)
    {
        animation->setAnimation(animations::AnimationType::Attack);
        attemptToAttack = true;
    }

    if (attemptToAttack and animation->getAnimationType() == animations::AnimationType::Attack &&
        animation->getCurrentAnimationProgressInPercents() >= 60)
    {
        attackStrategy->attack();
        attemptToAttack = false;
    }
}

}