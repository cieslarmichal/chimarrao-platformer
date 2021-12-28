#include "HealthComponent.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "ComponentOwner.h"

using namespace ::testing;
using namespace components::core;

class HealthComponentTest : public Test
{
public:
    const utils::Vector2f position{0.0, 11.0};
    const unsigned int initialHealthPoints{100};
    const unsigned int pointsToAdd1{20};
    const unsigned int pointsToAdd2{50};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position, "healthComponentTest", sharedContext};
    HealthComponent healthComponent{&componentOwner, initialHealthPoints};
};

TEST_F(HealthComponentTest, initialHealthPoints_shouldMatchWithGivenOne)
{
    ASSERT_EQ(healthComponent.getCurrentHealth(), initialHealthPoints);
    ASSERT_EQ(healthComponent.getMaximumHealth(), initialHealthPoints);
    ASSERT_FALSE(healthComponent.isDead());
}

TEST_F(HealthComponentTest,
       addedPointsWithCurrentHealthAreLowerThanMaximumHealth_shouldAddPointsToCurrentHealth)
{
    healthComponent.loseHealthPoints(50);

    healthComponent.gainHealthPoints(pointsToAdd1);

    ASSERT_EQ(healthComponent.getCurrentHealth(), 70);
}

TEST_F(HealthComponentTest,
       addedPointsWithCurrentHealthAreHigherThanMaximumHealth_shouldSetCurrentPointsToMaximum)
{
    healthComponent.loseHealthPoints(20);

    healthComponent.gainHealthPoints(pointsToAdd2);

    ASSERT_EQ(healthComponent.getCurrentHealth(), initialHealthPoints);
}

TEST_F(HealthComponentTest, lostPointsWithCurrentHealthAreLowerThanZero_shouldSetCurrentHealthPointsToZero)
{
    healthComponent.loseHealthPoints(120);

    ASSERT_EQ(healthComponent.getCurrentHealth(), 0);
}

TEST_F(HealthComponentTest,
       lostPointsWithCurrentHealthAreHigherThanZero_shouldSubtractPointsFromCurrentHealth)
{
    healthComponent.loseHealthPoints(20);

    ASSERT_EQ(healthComponent.getCurrentHealth(), 80);
}

TEST_F(HealthComponentTest, givenCurrentHealthEqualZero_shouldReturnDead)
{
    healthComponent.loseHealthPoints(100);

    ASSERT_TRUE(healthComponent.isDead());
}
