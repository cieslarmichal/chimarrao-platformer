#include "AnimationComponent.h"

namespace components
{
AnimationComponent::AnimationComponent(ComponentOwner* owner,
                                       std::shared_ptr<graphics::animation::Animator> animatorInit)
    : Component(owner), animator{std::move(animatorInit)}
{
}

void AnimationComponent::update(utils::DeltaTime deltaTime)
{
    animator->update(deltaTime);
}

void AnimationComponent::setAnimation(graphics::animation::AnimationType animationType)
{
    animator->setAnimation(animationType);
}

void AnimationComponent::setAnimation(graphics::animation::AnimationType animationType,
                                      graphics::animation::AnimationDirection animationDirection)
{
    animator->setAnimation(animationType, animationDirection);
}

void AnimationComponent::setAnimationDirection(graphics::animation::AnimationDirection animationDirection)
{
    animator->setAnimationDirection(animationDirection);
}

graphics::animation::AnimationType AnimationComponent::getAnimationType() const
{
    return animator->getAnimationType();
}

}