#include "ComponentOwner.h"

#include "gtest/gtest.h"

#include "HitBoxComponent.h"

using namespace ::testing;
using namespace components::core;

class ComponentOwnerTest : public Test
{
public:
    const utils::Vector2f initialPosition{0.0, 11.0};
    ComponentOwner componentOwner{initialPosition, "componentOwnerTest"};
};

TEST_F(ComponentOwnerTest, initialPosition_shouldBeSetFromConstructor)
{
    const auto actualPosition = componentOwner.transform->getPosition();

    ASSERT_EQ(actualPosition, initialPosition);
}

TEST_F(ComponentOwnerTest, addComponent_shouldBeValid)
{
    const auto hitBoxComponent = componentOwner.addComponent<HitBoxComponent>(utils::Vector2f{1, 1});

    ASSERT_TRUE(hitBoxComponent);
}

TEST_F(ComponentOwnerTest, addComponentTwoTimes_shouldBeReturnSameComponentPointer)
{
    const auto hitBoxComponent1 = componentOwner.addComponent<HitBoxComponent>(utils::Vector2f{5, 2});
    const auto hitBoxComponent2 = componentOwner.addComponent<HitBoxComponent>(utils::Vector2f{1, 2});

    ASSERT_EQ(hitBoxComponent1, hitBoxComponent2);
}

TEST_F(ComponentOwnerTest, getComponentWhenComponentNotCreated_shouldReturnNullptr)
{
    const auto hitBoxComponent = componentOwner.getComponent<HitBoxComponent>();

    ASSERT_FALSE(hitBoxComponent);
}

TEST_F(ComponentOwnerTest, getComponentWhenComponentCreated_shouldReturnValidComponentPointer)
{
    const auto addedHitBoxComponent = componentOwner.addComponent<HitBoxComponent>(utils::Vector2f{8, 0});

    const auto hitBoxComponentByGet = componentOwner.getComponent<HitBoxComponent>();

    ASSERT_EQ(addedHitBoxComponent, hitBoxComponentByGet);
}

TEST_F(ComponentOwnerTest, shouldDisableComponents)
{
    const auto hitBoxComponent = componentOwner.addComponent<HitBoxComponent>(utils::Vector2f{3, 0});
    auto transformComponent = componentOwner.transform;

    componentOwner.disable();

    ASSERT_FALSE(hitBoxComponent->isEnabled());
    ASSERT_FALSE(transformComponent->isEnabled());
}

TEST_F(ComponentOwnerTest, shouldEnableComponents)
{
    const auto hitBoxComponent = componentOwner.addComponent<HitBoxComponent>(utils::Vector2f{3, 0});
    auto transformComponent = componentOwner.transform;
    componentOwner.disable();

    componentOwner.enable();

    ASSERT_TRUE(hitBoxComponent->isEnabled());
    ASSERT_TRUE(transformComponent->isEnabled());
}