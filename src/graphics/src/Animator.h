#pragma once

#include "AnimationType.h"
#include "DeltaTime.h"

namespace graphics
{
class Animator
{
public:
    virtual ~Animator() = default;

    virtual void update(const utils::DeltaTime&) = 0;
    virtual void setAnimation(AnimationType) = 0;
    virtual void setAnimation(AnimationType, AnimationDirection) = 0;
};
}