#include "FollowerComponent.h"

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
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner followerOwner1{position, "followerComponentTest1", sharedContext};
    ComponentOwner followerOwner2{position, "followerComponentTest2", sharedContext};
    ComponentOwner followedOwner1{positionOnLeft, "followerComponentTest3", sharedContext};
    ComponentOwner followedOwner2{positionOnRight, "followerComponentTest4", sharedContext};
    FollowerComponent follower1{&followerOwner1, &followedOwner1};
    FollowerComponent follower2{&followerOwner2, &followedOwner2};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();
    StrictMock<input::InputMock> input;
};

TEST_F(FollowerComponentTest,
       loadDependentComponentsWithoutAnimatorComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutAnimator{position, "componentOwnerWithoutAnimator", sharedContext};
    FollowerComponent followerComponentWithoutAnimator{&componentOwnerWithoutAnimator, &followedOwner1};
    componentOwnerWithoutAnimator.addComponent<VelocityComponent>();

    ASSERT_THROW(followerComponentWithoutAnimator.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(FollowerComponentTest,
       loadDependentComponentsWithoutVelocityComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutVelocity{position, "componentOwnerWithoutVelocity", sharedContext};
    FollowerComponent followerComponentWithoutVelocity{&componentOwnerWithoutVelocity, &followedOwner1};
    componentOwnerWithoutVelocity.addComponent<AnimationComponent>(animator);

    ASSERT_THROW(followerComponentWithoutVelocity.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

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