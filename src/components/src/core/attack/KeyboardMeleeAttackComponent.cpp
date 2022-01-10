#include "KeyboardMeleeAttackComponent.h"

#include "HealthComponent.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

KeyboardMeleeAttackComponent::KeyboardMeleeAttackComponent(ComponentOwner* owner,
                                                 std::shared_ptr<AttackStrategy> attackStrategy)
    : Component{owner}, attackStrategy{std::move(attackStrategy)}

{
}

void KeyboardMeleeAttackComponent::loadDependentComponents()
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

void KeyboardMeleeAttackComponent::update(utils::DeltaTime, const input::Input& input)
{
    if (not enabled)
    {
        return;
    }

    if (input.isKeyPressed(input::InputKey::Space) and
        animation->getAnimationType() != animations::AnimationType::Attack and
        animation->getAnimationType() != animations::AnimationType::Sleep)
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