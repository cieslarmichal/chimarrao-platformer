#include "AnimationComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "DeltaTime.h"

using namespace ::testing;
using namespace components::core;
using namespace animations;

class AnimationComponentTest : public Test
{
public:
    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{0.0, 11.0};
    const AnimationType animationType{AnimationType::Jump};
    const AnimationDirection animationDirection{AnimationDirection::Left};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position, "animationComponentTest", sharedContext};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();
    StrictMock<input::InputMock> input;
    AnimationComponent animationComponent{&componentOwner, animator};
};

TEST_F(AnimationComponentTest, update)
{
    EXPECT_CALL(*animator, update(deltaTime));

    animationComponent.update(deltaTime, input);
}

TEST_F(AnimationComponentTest, setAnimationByAnimationType)
{
    EXPECT_CALL(*animator, setAnimation(animationType));

    animationComponent.setAnimation(animationType);
}

TEST_F(AnimationComponentTest, forceAnimation)
{
    EXPECT_CALL(*animator, forceAnimation(animationType));

    animationComponent.forceAnimation(animationType);
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

TEST_F(AnimationComponentTest, getAnimationProgressInPercents)
{
    const unsigned int dummyPercents = 50;
    EXPECT_CALL(*animator, getCurrentAnimationProgressInPercents()).WillOnce(Return(dummyPercents));

    ASSERT_EQ(animationComponent.getCurrentAnimationProgressInPercents(), dummyPercents);
}