#include "ExplodeOnCollisionComponent.h"

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

ExplodeOnCollisionComponent::ExplodeOnCollisionComponent(ComponentOwner* owner, unsigned int damage)
    : Component{owner}, damage{damage}, explosionStarted{false}, target{nullptr}
{
}

void ExplodeOnCollisionComponent::loadDependentComponents()
{
    animationComponent = owner->getComponent<AnimationComponent>();
    if (animationComponent)
    {
        animationComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "ExplodeOnCollisionComponent: AnimationComponent not found"};
    }

    boxColliderComponent = owner->getComponent<BoxColliderComponent>();
    if (boxColliderComponent)
    {
        boxColliderComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "ExplodeOnCollisionComponent: BoxColliderComponent not found"};
    }
}

void ExplodeOnCollisionComponent::update(utils::DeltaTime, const input::Input&)
{
    if (not target)
    {
        target = boxColliderComponent->getCurrentColliderOnXAxis();
    }

    if (target and not explosionStarted)
    {
        animationComponent->setAnimation(animations::AnimationType::Explode);
        explosionStarted = true;
    }

    if (explosionStarted and animationComponent->getCurrentAnimationProgressInPercents() == 100)
    {
        if (auto health = target->getComponent<HealthComponent>())
        {
            health->loseHealthPoints(damage);
        }

        owner->remove();
    }
}
}