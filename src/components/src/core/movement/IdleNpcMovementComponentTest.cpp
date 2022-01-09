#include "IdleNpcMovementComponent.h"

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

class IdleNpcMovementTest : public Test
{
public:
    IdleNpcMovementTest()
    {
        idleOwner1.addComponent<AnimationComponent>(animator);
        idleOwner1.addComponent<VelocityComponent>(6);
        idleNpcMovementComponent1.loadDependentComponents();

        idleOwner2.addComponent<AnimationComponent>(animator);
        idleOwner2.addComponent<VelocityComponent>(6);
        idleNpcMovementComponent2.loadDependentComponents();
    }

    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{50.0, 11.0};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner idleOwner1{position, "idleOwner1", sharedContext};
    ComponentOwner idleOwner2{position, "idleOwner2", sharedContext};
    ComponentOwner player1{positionOnLeft, "player1", sharedContext};
    ComponentOwner player2{positionOnRight, "player2", sharedContext};
    IdleNpcMovementComponent idleNpcMovementComponent1{&idleOwner1, &player1};
    IdleNpcMovementComponent idleNpcMovementComponent2{&idleOwner2, &player2};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();
    StrictMock<input::InputMock> input;
};

TEST_F(IdleNpcMovementTest,
       loadDependentComponentsWithoutAnimatorComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutAnimator{position, "componentOwnerWithoutAnimator", sharedContext};
    IdleNpcMovementComponent idleComponentWithoutAnimator{&componentOwnerWithoutAnimator, &player1};
    componentOwnerWithoutAnimator.addComponent<VelocityComponent>(6);

    ASSERT_THROW(idleComponentWithoutAnimator.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(IdleNpcMovementTest,
       loadDependentComponentsWithoutVelocityComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutVelocity{position, "componentOwnerWithoutVelocity", sharedContext};
    IdleNpcMovementComponent idleComponentWithoutVelocity{&componentOwnerWithoutVelocity, &player1};
    componentOwnerWithoutVelocity.addComponent<AnimationComponent>(animator);

    ASSERT_THROW(idleComponentWithoutVelocity.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(IdleNpcMovementTest, followedTargetOnLeft_shouldTurnLeft)
{
    idleNpcMovementComponent1.blockMoveDown();
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Left));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Idle));

    idleNpcMovementComponent1.update(deltaTime, input);
    idleNpcMovementComponent1.lateUpdate(deltaTime, input);
}

TEST_F(IdleNpcMovementTest, followedTargetOnRight_shouldTurnRight)
{
    idleNpcMovementComponent2.blockMoveDown();
    EXPECT_CALL(*animator, setAnimationDirection(AnimationDirection::Right));
    EXPECT_CALL(*animator, setAnimation(AnimationType::Idle));

    idleNpcMovementComponent2.update(deltaTime, input);
    idleNpcMovementComponent2.lateUpdate(deltaTime, input);
}