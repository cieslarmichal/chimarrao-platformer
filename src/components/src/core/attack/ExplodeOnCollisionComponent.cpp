#include "ExplodeOnCollisionComponent.h"

#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

ExplodeOnCollisionComponent::ExplodeOnCollisionComponent(ComponentOwner* owner, unsigned int damage)
    : Component{owner}, damage{damage}, explosionStarted{false}, explosionFinished{false}, target{nullptr}
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

    movementComponent = owner->getComponent<MovementComponent>();
    if (movementComponent)
    {
        movementComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "ExplodeOnCollisionComponent: MovementComponent not found"};
    }
}

void ExplodeOnCollisionComponent::update(utils::DeltaTime, const input::Input&)
{
    if (explosionFinished)
    {
        return;
    }

    const auto validTarget = boxColliderComponent->getCurrentColliderOnXAxis();

    if (validTarget and not explosionStarted)
    {
        target = validTarget;
        animationComponent->setAnimation(animations::AnimationType::Explode);
        explosionStarted = true;
        movementComponent->lock();

        if (auto health = target->getComponent<HealthComponent>())
        {
            health->loseHealthPoints(damage);
        }
    }

    if (explosionStarted and animationComponent->getCurrentAnimationProgressInPercents() == 100)
    {
        explosionFinished = true;
        owner->remove();
    }
}
}