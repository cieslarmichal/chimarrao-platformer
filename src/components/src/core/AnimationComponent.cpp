#include "AnimationComponent.h"

namespace components::core
{
AnimationComponent::AnimationComponent(ComponentOwner* ownerInit,
                                       std::shared_ptr<animations::Animator> animatorInit)
    : Component(ownerInit), animator{std::move(animatorInit)}
{
}

void AnimationComponent::update(utils::DeltaTime deltaTime, const input::Input&)
{
    animator->update(deltaTime);
}

void AnimationComponent::setAnimation(animations::AnimationType animationType)
{
    std::cerr << animationType<< std::endl;
    animator->setAnimation(animationType);
}

void AnimationComponent::setAnimation(animations::AnimationType animationType,
                                      animations::AnimationDirection animationDirection)
{
    animator->setAnimation(animationType, animationDirection);
}

void AnimationComponent::setAnimationDirection(animations::AnimationDirection animationDirection)
{
    animator->setAnimationDirection(animationDirection);
}

animations::AnimationType AnimationComponent::getAnimationType() const
{
    return animator->getAnimationType();
}

animations::AnimationDirection AnimationComponent::getAnimationDirection() const
{
    return animator->getAnimationDirection();
}

}