#include "AnimationComponent.h"

#include "gtest/gtest.h"

#include "animation/AnimatorMock.h"

#include "ComponentOwner.h"
#include "DeltaTime.h"

using namespace ::testing;
using namespace components;
using namespace graphics;
using namespace animation;

class AnimationComponentTest : public Test
{
public:
    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{0.0, 11.0};
    const AnimationType animationType{AnimationType::Jump};
    const AnimationDirection animationDirection{AnimationDirection::Left};
    ComponentOwner componentOwner{position};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();

    AnimationComponent animationComponent{&componentOwner, animator};
};

TEST_F(AnimationComponentTest, update)
{
    EXPECT_CALL(*animator, update(deltaTime));

    animationComponent.update(deltaTime);
}

TEST_F(AnimationComponentTest, setAnimationByAnimationType)
{
    EXPECT_CALL(*animator, setAnimation(animationType));

    animationComponent.setAnimation(animationType);
}

TEST_F(AnimationComponentTest, setAnimationByAnimationTypeAndAnimationDirection)
{
    EXPECT_CALL(*animator, setAnimation(animationType, animationDirection));

    animationComponent.setAnimation(animationType, animationDirection);
}

TEST_F(AnimationComponentTest, setAnimationDirection)
{
    EXPECT_CALL(*animator, setAnimationDirection(animationDirection));

    animationComponent.setAnimationDirection(animationDirection);
}

TEST_F(AnimationComponentTest, getAnimationType)
{
    EXPECT_CALL(*animator, getAnimationType()).WillOnce(Return(animationType));

    const auto actualAnimationType = animationComponent.getAnimationType();

    ASSERT_EQ(actualAnimationType, animationType);
}

TEST_F(AnimationComponentTest, getAnimationDirection)
{
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animationDirection));

    const auto actualAnimationDirection = animationComponent.getAnimationDirection();

    ASSERT_EQ(actualAnimationDirection, animationDirection);
}