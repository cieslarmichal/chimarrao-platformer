#include "FreeFallMovementComponent.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "VelocityComponent.h"
#include "exceptions/DependentComponentNotFound.h"

using namespace components::core;
using namespace ::testing;

class FreeFallMovementTest : public Test
{
public:
    FreeFallMovementTest()
    {
        fallingOwner.addComponent<VelocityComponent>(6);
        freeFallMovementComponent.loadDependentComponents();
    }

    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{50.0, 11.0};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner fallingOwner{position, "idleOwner1", sharedContext};
    FreeFallMovementComponent freeFallMovementComponent{&fallingOwner};
    StrictMock<input::InputMock> input;
};

TEST_F(FreeFallMovementTest,
       loadDependentComponentsWithoutVelocityComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutVelocity{position, "componentOwnerWithoutVelocity", sharedContext};
    FreeFallMovementComponent freeFallComponentWithoutVelocity{&componentOwnerWithoutVelocity};

    ASSERT_THROW(freeFallComponentWithoutVelocity.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(FreeFallMovementTest, canMoveDown_shouldMoveDown)
{
    freeFallMovementComponent.update(deltaTime, input);
    freeFallMovementComponent.lateUpdate(deltaTime, input);

    const auto positionYAfterFall = position.y + 25.f * deltaTime.count() * deltaTime.count();
    ASSERT_EQ(fallingOwner.transform->getPosition().x, position.x);
    ASSERT_EQ(fallingOwner.transform->getPosition().y, positionYAfterFall);
}

TEST_F(FreeFallMovementTest, canNotMoveDown_shouldNotMoveDown)
{
    freeFallMovementComponent.blockMoveDown();

    freeFallMovementComponent.update(deltaTime, input);
    freeFallMovementComponent.lateUpdate(deltaTime, input);

    ASSERT_EQ(fallingOwner.transform->getPosition(), position);
}