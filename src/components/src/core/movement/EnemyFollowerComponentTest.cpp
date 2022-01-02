#include "EnemyFollowerComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"

#include "AnimationComponent.h"
#include "ComponentOwner.h"
#include "VelocityComponent.h"
#include "exceptions/DependentComponentNotFound.h"

using namespace components::core;
using namespace animations;
using namespace ::testing;

class EnemyFollowerComponentTest : public Test
{
public:
    EnemyFollowerComponentTest()
    {
        followerOwner1.addComponent<AnimationComponent>(animator);
        followerOwner1.addComponent<VelocityComponent>();
        follower1.loadDependentComponents();

        followerOwner2.addComponent<AnimationComponent>(animator);
        followerOwner2.addComponent<VelocityComponent>();
        follower2.loadDependentComponents();

        followerOwner3.addComponent<AnimationComponent>(animator);
        followerOwner3.addComponent<VelocityComponent>();
        follower3.loadDependentComponents();

        followerOwner4.addComponent<AnimationComponent>(animator);
        followerOwner4.addComponent<VelocityComponent>();
        follower4.loadDependentComponents();
    }

    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{50.0, 11.0};
    const utils::Vector2f positionOnRightInRange{56.0, 11.0};
    const utils::Vector2f positionOnLeftInRange{45.0, 11.0};
    const utils::Vector2f positionOnLeftOutOfRange{3, 11.0};
    const utils::Vector2f positionOnRightOutOfRange{154, 60};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner followerOwner1{position, "followerComponentTest1", sharedContext};
    ComponentOwner followerOwner2{position, "followerComponentTest2", sharedContext};
    ComponentOwner followerOwner3{position, "followerComponentTest3", sharedContext};
    ComponentOwner followerOwner4{position, "followerComponentTest4", sharedContext};
    ComponentOwner followedOwnerInRangeOnLeft{positionOnLeftInRange, "followerComponentTest5", sharedContext};
    ComponentOwner followedOwnerInRangeOnRight{positionOnRightInRange, "followerComponentTest6",
                                               sharedContext};
    ComponentOwner followedOwnerOutOfRangeOnLeft{positionOnLeftOutOfRange, "followerComponentTest7",
                                                 sharedContext};
    ComponentOwner followedOwnerOutOfRangeOnRight{positionOnRightOutOfRange, "followerComponentTest8",
                                                  sharedContext};
    EnemyFollowerComponent follower1{&followerOwner1, &followedOwnerInRangeOnLeft};
    EnemyFollowerComponent follower2{&followerOwner2, &followedOwnerInRangeOnRight};
    EnemyFollowerComponent follower3{&followerOwner3, &followedOwnerOutOfRangeOnLeft};
    EnemyFollowerComponent follower4{&followerOwner4, &followedOwnerOutOfRangeOnRight};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();
    StrictMock<input::InputMock> input;
};

TEST_F(EnemyFollowerComponentTest,
       loadDependentComponentsWithoutAnimatorComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutAnimator{position, "componentOwnerWithoutAnimator", sharedContext};
    EnemyFollowerComponent followerComponentWithoutAnimator{&componentOwnerWithoutAnimator,
                                                            &followedOwnerInRangeOnLeft};
    componentOwnerWithoutAnimator.addComponent<VelocityComponent>();

    ASSERT_THROW(followerComponentWithoutAnimator.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(EnemyFollowerComponentTest,
       loadDependentComponentsWithoutVelocityComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutVelocity{position, "componentOwnerWithoutVelocity", sharedContext};
    EnemyFollowerComponent followerComponentWithoutVelocity{&componentOwnerWithoutVelocity,
                                                            &followedOwnerInRangeOnLeft};
    componentOwnerWithoutVelocity.addComponent<AnimationComponent>(animator);

    ASSERT_THROW(followerComponentWithoutVelocity.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(EnemyFollowerComponentTest, followedTargetInRangeOnLeft_shouldMoveLeft)
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

TEST_F(EnemyFollowerComponentTest, followedTargetOutOfRangeOnLeft_shouldNotMoveLeft)
{
    follower3.blockMoveDown();
    const auto positionBefore = follower3.getOwner().transform->getPosition();
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Idle));

    follower3.update(deltaTime, input);
    follower3.lateUpdate(deltaTime, input);

    const auto positionAfter = follower3.getOwner().transform->getPosition();
    ASSERT_EQ(positionAfter.x, positionBefore.x);
}

TEST_F(EnemyFollowerComponentTest, followedTargetInRangeOnRight_shouldMoveRight)
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

TEST_F(EnemyFollowerComponentTest, followedTargetOutOfRangeOnRight_shouldNotMoveRight)
{
    follower4.blockMoveDown();
    const auto positionBefore = follower4.getOwner().transform->getPosition();
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Idle));

    follower4.update(deltaTime, input);
    follower4.lateUpdate(deltaTime, input);

    const auto positionAfter = follower4.getOwner().transform->getPosition();
    ASSERT_EQ(positionAfter.x, positionBefore.x);
}