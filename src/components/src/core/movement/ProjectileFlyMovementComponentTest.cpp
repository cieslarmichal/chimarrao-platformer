
#include "ProjectileFlyMovementComponent.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "RendererPoolMock.h"

#include "AnimationComponent.h"
#include "ComponentOwner.h"
#include "VelocityComponent.h"
#include "exceptions/DependentComponentNotFound.h"
#include "AnimatorMock.h"

using namespace components::core;
using namespace animations;
using namespace ::testing;

class ProjectileFlyMovementComponentTest : public Test
{
public:
    ProjectileFlyMovementComponentTest()
    {
        owner1.addComponent<VelocityComponent>(6);
        owner1.addComponent<AnimationComponent>(animator);
        leftMovementComponent.loadDependentComponents();

        owner2.addComponent<VelocityComponent>(6);
        owner2.addComponent<AnimationComponent>(animator);
        rightMovementComponent.loadDependentComponents();
    }

    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{50.0, 11.0};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner owner1{position, "KeyboardHorizontalFlyMovementComponentTest1", sharedContext};
    ComponentOwner owner2{position, "KeyboardHorizontalFlyMovementComponentTest2", sharedContext};
    ProjectileFlyMovementComponent leftMovementComponent{&owner1, animations::AnimationDirection::Left};
    ProjectileFlyMovementComponent rightMovementComponent{&owner2, animations::AnimationDirection::Right};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();
    StrictMock<input::InputMock> input;
};

TEST_F(ProjectileFlyMovementComponentTest,
       loadDependentComponentsWithoutAnimatorComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutAnimator{position, "componentOwnerWithoutAnimator", sharedContext};
    ProjectileFlyMovementComponent movementComponentWithoutAnimator{
        &componentOwnerWithoutAnimator, animations::AnimationDirection::Right};
    componentOwnerWithoutAnimator.addComponent<VelocityComponent>(100);

    ASSERT_THROW(movementComponentWithoutAnimator.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(ProjectileFlyMovementComponentTest,
       loadDependentComponentsWithoutVelocityComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutVelocity{position, "componentOwnerWithoutVelocity", sharedContext};
    ProjectileFlyMovementComponent movementComponentWithoutVelocity{&componentOwnerWithoutVelocity, animations::AnimationDirection::Right};
    componentOwnerWithoutVelocity.addComponent<AnimationComponent>(animator);
    ASSERT_THROW(movementComponentWithoutVelocity.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(ProjectileFlyMovementComponentTest, movementComponentInitializedWithLeftDirection_shouldMoveLeft)
{
    EXPECT_CALL(*animator, setAnimation(AnimationType::Fly, animations::AnimationDirection::Left));
    const auto positionBefore = owner1.transform->getPosition();

    leftMovementComponent.update(deltaTime, input);
    leftMovementComponent.lateUpdate(deltaTime, input);

    const auto positionAfter = owner1.transform->getPosition();
    ASSERT_TRUE(positionAfter.x < positionBefore.x);
}

TEST_F(ProjectileFlyMovementComponentTest, movementComponentInitializedWithRightDirection_shouldMoveRight)
{
    EXPECT_CALL(*animator, setAnimation(AnimationType::Fly, animations::AnimationDirection::Right));
    const auto positionBefore = owner2.transform->getPosition();

    rightMovementComponent.update(deltaTime, input);
    rightMovementComponent.lateUpdate(deltaTime, input);

    const auto positionAfter = owner2.transform->getPosition();
    ASSERT_TRUE(positionAfter.x > positionBefore.x);
}