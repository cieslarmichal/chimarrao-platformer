#pragma once

#include "AnimationType.h"
#include "AnimationDirection.h"
#include "DeltaTime.h"

namespace graphics::animation
{
class Animator
{
public:
    virtual ~Animator() = default;

    virtual void update(const utils::DeltaTime&) = 0;
    virtual void setAnimation(AnimationType) = 0;
    virtual void setAnimation(AnimationType, AnimationDirection) = 0;
    virtual void setAnimationDirection(AnimationDirection) = 0;
    virtual AnimationType getAnimationType() const = 0;
};
}