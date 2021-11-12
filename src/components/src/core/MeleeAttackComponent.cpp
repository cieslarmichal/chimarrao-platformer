#include "MeleeAttackComponent.h"

#include "HealthComponent.h"
#include "exceptions/DependentComponentNotFound.h"

namespace components::core
{

MeleeAttackComponent::MeleeAttackComponent(ComponentOwner* owner, std::shared_ptr<physics::RayCast> rayCast)
    : Component{owner}, rayCast{std::move(rayCast)}

{
}

void MeleeAttackComponent::loadDependentComponents()
{
    animation = owner->getComponent<AnimationComponent>();
    if (animation)
    {
        animation->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{
            "MeleeAttackComponent: Animation component not found"};
    }

    directionComponent = owner->getComponent<DirectionComponent>();
    if (directionComponent)
    {
        directionComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"MeleeAttackComponent: DirectionComponent not found"};
    }

    boxColliderComponent = owner->getComponent<BoxColliderComponent>();
    if (boxColliderComponent)
    {
        boxColliderComponent->loadDependentComponents();
    }
    else
    {
        throw exceptions::DependentComponentNotFound{"MeleeAttackComponent: BoxColliderComponent not found"};
    }
}

void MeleeAttackComponent::update(utils::DeltaTime, const input::Input& input)
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
        attack();
        attemptToAttack = false;
    }
}

void MeleeAttackComponent::attack() const
{
    const auto heading = directionComponent->getHeading();
    const auto& ownerPosition = owner->transform->getPosition();
    const auto size = boxColliderComponent->getSize();

    const auto startPoint =
        utils::Vector2f{ownerPosition.x + (size.x / 2.f), ownerPosition.y + (size.y / 2.f)};
    const auto endPoint =
        utils::Vector2f{startPoint.x + (static_cast<float>(heading.x) * range), startPoint.y};

    const auto result = rayCast->cast(startPoint, endPoint, owner->getId(), 2);

    if (result.collision)
    {
        if (auto health = result.collision->getComponent<HealthComponent>())
        {
            health->loseHealthPoints(damage);
        }
    }
}

}