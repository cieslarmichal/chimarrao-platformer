#pragma once

#include "gmock/gmock.h"

#include "Animator.h"

namespace graphics::animation
{
class AnimatorMock : public Animator
{
public:
    MOCK_METHOD(void, update, (const utils::DeltaTime&), (override));
    MOCK_METHOD(void, setAnimation, (AnimationType), (override));
    MOCK_METHOD(void, setAnimation, (AnimationType, AnimationDirection), (override));
    MOCK_METHOD(void, setAnimationDirection, (AnimationDirection), (override));
    MOCK_METHOD(AnimationType, getAnimationType, (), (const override));
};
}
