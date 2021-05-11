#include "VelocityComponent.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace components::core;

class VelocityComponentTest : public Test
{
public:
    const float x{1.0};
    const float y{2.0};
    const utils::Vector2f position1{0.0, 10.0};
    const utils::Vector2f velocity{10.0, 15.0};
    const utils::Vector2f maximalVelocity{100.f, 15.f};
    const utils::Vector2f velocityBiggerThanMaximal{150.0, 15.0};
    ComponentOwner componentOwner{position1, "velocityComponentTest"};
    VelocityComponent velocityComponent{&componentOwner};
};

TEST_F(VelocityComponentTest, initialComponent_shouldHaveZeroVelocity)
{
    ASSERT_EQ(velocityComponent.getVelocity(), utils::Vector2f(0, 0));
}

TEST_F(VelocityComponentTest, shouldSetVelocity)
{
    velocityComponent.setVelocity(velocity);

    ASSERT_EQ(velocityComponent.getVelocity(), velocity);
}

TEST_F(VelocityComponentTest, givenVelocityBiggerThanMaximal_shouldTrimVelocityToMaximal)
{
    velocityComponent.setVelocity(velocityBiggerThanMaximal);

    ASSERT_EQ(velocityComponent.getVelocity(), maximalVelocity);
}