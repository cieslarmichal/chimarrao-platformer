#include "ArtificialIntelligenceAttackComponent.h"

#include "core/exceptions/DependentComponentNotFound.h"

namespace components::core
{

ArtificialIntelligenceAttackComponent::ArtificialIntelligenceAttackComponent(ComponentOwner* owner,ComponentOwner* target,
                                                                             std::shared_ptr<AttackStrategy> attackStrategy)
    : Component{owner}, target{target}, attackStrategy{std::move(attackStrategy)}

{
}

void ArtificialIntelligenceAttackComponent::loadDependentComponents()
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

void ArtificialIntelligenceAttackComponent::update(utils::DeltaTime, const input::Input&)
{
    if (std::abs(target->transform->getPosition().x - owner->transform->getPosition().x) < 4.f and
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