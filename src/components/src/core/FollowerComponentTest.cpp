#include "FollowerComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "InputMock.h"

#include "AnimationComponent.h"
#include "ComponentOwner.h"
#include "VelocityComponent.h"

using namespace components::core;
using namespace animations;
using namespace ::testing;

namespace
{
const utils::Vector2f positionOnLeft{3, 60};
const utils::Vector2f positionOnRight = {154, 60};
}

class FollowerComponentTest : public Test
{
public:
    FollowerComponentTest()
    {
        followerOwner1.addComponent<AnimationComponent>(animator);
        followerOwner1.addComponent<VelocityComponent>();
        follower1.loadDependentComponents();

        followerOwner2.addComponent<AnimationComponent>(animator);
        followerOwner2.addComponent<VelocityComponent>();
        follower2.loadDependentComponents();
    }

    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{50.0, 11.0};
    ComponentOwner followerOwner1{position, "followerComponentTest1"};
    ComponentOwner followerOwner2{position, "followerComponentTest2"};
    ComponentOwner followedOwner1{positionOnLeft, "followerComponentTest3"};
    ComponentOwner followedOwner2{positionOnRight, "followerComponentTest4"};
    FollowerComponent follower1{&followerOwner1, &followedOwner1};
    FollowerComponent follower2{&followerOwner2, &followedOwner2};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();
    StrictMock<input::InputMock> input;
};

TEST_F(FollowerComponentTest, followedTargetOnLeft_shouldMoveLeft)
{
    follower1.blockMoveDown();
    const auto positionBefore = follower1.getOwner().transform->getPosition();
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));

    follower1.update(deltaTime, input);
    follower1.lateUpdate(deltaTime, input);

    const auto positionAfter = follower1.getOwner().transform->getPosition();
    ASSERT_TRUE(positionAfter.x < positionBefore.x);
}

TEST_F(FollowerComponentTest, followedTargetOnRight_shouldMoveRight)
{
    follower2.blockMoveDown();
    const auto positionBefore = follower2.getOwner().transform->getPosition();
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Walk));

    follower2.update(deltaTime, input);
    follower2.lateUpdate(deltaTime, input);

    const auto positionAfter = follower2.getOwner().transform->getPosition();
    ASSERT_TRUE(positionAfter.x > positionBefore.x);
}