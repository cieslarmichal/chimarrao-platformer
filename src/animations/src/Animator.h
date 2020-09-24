#pragma once

#include "AnimationDirection.h"
#include "AnimationType.h"
#include "DeltaTime.h"

namespace animations
{
using AnimationChanged = bool;

class Animator
{
public:
    virtual ~Animator() = default;

    virtual AnimationChanged update(const utils::DeltaTime&) = 0;
    virtual void setAnimation(AnimationType) = 0;
    virtual void setAnimation(AnimationType, AnimationDirection) = 0;
    virtual void setAnimationDirection(AnimationDirection) = 0;
    virtual AnimationType getAnimationType() const = 0;
    virtual AnimationDirection getAnimationDirection() const = 0;
};
}