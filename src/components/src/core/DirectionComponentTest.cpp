#include "DirectionComponent.h"

#include "gtest/gtest.h"

#include "core/exceptions/DependentComponentNotFound.h"

using namespace ::testing;
using namespace components::core;

class DirectionComponentTest : public Test
{
public:
    DirectionComponentTest()
    {
        velocityComponent = componentOwner.addComponent<VelocityComponent>();
        directionComponent.loadDependentComponents();
    }

    const float x{1.0};
    const float y{2.0};
    const utils::Vector2f position1{0.0, 10.0};
    const utils::Vector2f velocityDirectedRight{10.0, 0};
    const utils::Vector2f velocityDirectedLeft{-10.0, 0};
    ComponentOwner componentOwner{position1, "DirectionComponentTest"};
    std::shared_ptr<VelocityComponent> velocityComponent;
    DirectionComponent directionComponent{&componentOwner};
};

TEST_F(DirectionComponentTest,
       loadDependentComponentsWithoutVelocityComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutVelocity{position1, "componentOwnerWithoutVelocity"};
    DirectionComponent directionComponentWithoutAnimator{&componentOwnerWithoutVelocity};

    ASSERT_THROW(directionComponentWithoutAnimator.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(DirectionComponentTest, givenVelocityGoingRight_shouldReturnRightDirection)
{
    velocityComponent->setVelocity(velocityDirectedRight);

    ASSERT_EQ(directionComponent.getDirection(), animations::AnimationDirection::Right);
}

TEST_F(DirectionComponentTest, givenVelocityGoingRight_shouldReturnLeftDirection)
{
    velocityComponent->setVelocity(velocityDirectedLeft);

    ASSERT_EQ(directionComponent.getDirection(), animations::AnimationDirection::Left);
}