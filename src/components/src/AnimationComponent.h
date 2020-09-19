#pragma once

#include "Component.h"
#include "animation/Animator.h"

namespace components
{
class AnimationComponent : public Component
{
public:
    AnimationComponent(ComponentOwner*, std::shared_ptr<graphics::animation::Animator>);

    void update(utils::DeltaTime) override;
    void setAnimation(graphics::animation::AnimationType);
    void setAnimation(graphics::animation::AnimationType, graphics::animation::AnimationDirection);
    void setAnimationDirection(graphics::animation::AnimationDirection);
    graphics::animation::AnimationType getAnimationType() const;
    graphics::animation::AnimationDirection getAnimationDirection() const;

private:
    std::shared_ptr<graphics::animation::Animator> animator;
};
}
