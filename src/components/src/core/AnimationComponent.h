#pragma once

#include "Animator.h"
#include "Component.h"

namespace components::core
{
class AnimationComponent : public Component
{
public:
    AnimationComponent(ComponentOwner*, std::shared_ptr<animations::Animator>);

    void update(utils::DeltaTime, const input::Input&) override;
    void forceAnimation(animations::AnimationType);
    void setAnimation(animations::AnimationType);
    void setAnimation(animations::AnimationType, animations::AnimationDirection);
    void setAnimationDirection(animations::AnimationDirection);
    animations::AnimationType getAnimationType() const;
    animations::AnimationDirection getAnimationDirection() const;
    unsigned int getCurrentAnimationProgressInPercents();

private:
    std::shared_ptr<animations::Animator> animator;
};
}
