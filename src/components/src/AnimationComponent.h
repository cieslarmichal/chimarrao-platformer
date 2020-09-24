#pragma once

#include "Animator.h"
#include "Component.h"

namespace components
{
class AnimationComponent : public Component
{
public:
    AnimationComponent(ComponentOwner*, std::shared_ptr<animations::Animator>);

    void update(utils::DeltaTime) override;
    void setAnimation(animations::AnimationType);
    void setAnimation(animations::AnimationType, animations::AnimationDirection);
    void setAnimationDirection(animations::AnimationDirection);
    animations::AnimationType getAnimationType() const;
    animations::AnimationDirection getAnimationDirection() const;

private:
    std::shared_ptr<animations::Animator> animator;
};
}
