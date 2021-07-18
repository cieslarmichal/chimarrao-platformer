#include "ComponentOwner.h"

#include "gtest/gtest.h"

#include "BoxColliderComponent.h"

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
    const auto boxColliderComponent =
        componentOwner.addComponent<BoxColliderComponent>(utils::Vector2f{1, 1});

    ASSERT_TRUE(boxColliderComponent);
}

TEST_F(ComponentOwnerTest, addComponentTwoTimes_shouldBeReturnSameComponentPointer)
{
    const auto boxColliderComponent1 =
        componentOwner.addComponent<BoxColliderComponent>(utils::Vector2f{5, 2});
    const auto boxColliderComponent2 =
        componentOwner.addComponent<BoxColliderComponent>(utils::Vector2f{1, 2});

    ASSERT_EQ(boxColliderComponent1, boxColliderComponent2);
}

TEST_F(ComponentOwnerTest, getComponentWhenComponentNotCreated_shouldReturnNullptr)
{
    const auto boxColliderComponent = componentOwner.getComponent<BoxColliderComponent>();

    ASSERT_FALSE(boxColliderComponent);
}

TEST_F(ComponentOwnerTest, getComponentWhenComponentCreated_shouldReturnValidComponentPointer)
{
    const auto addedBoxColliderComponent =
        componentOwner.addComponent<BoxColliderComponent>(utils::Vector2f{8, 0});

    const auto boxColliderComponentByGet = componentOwner.getComponent<BoxColliderComponent>();

    ASSERT_EQ(addedBoxColliderComponent, boxColliderComponentByGet);
}

TEST_F(ComponentOwnerTest, shouldDisableComponents)
{
    const auto boxColliderComponent =
        componentOwner.addComponent<BoxColliderComponent>(utils::Vector2f{3, 0});
    auto transformComponent = componentOwner.transform;

    componentOwner.disable();

    ASSERT_FALSE(boxColliderComponent->isEnabled());
    ASSERT_FALSE(transformComponent->isEnabled());
}

TEST_F(ComponentOwnerTest, shouldEnableComponents)
{
    const auto boxColliderComponent =
        componentOwner.addComponent<BoxColliderComponent>(utils::Vector2f{3, 0});
    auto transformComponent = componentOwner.transform;
    componentOwner.disable();

    componentOwner.enable();

    ASSERT_TRUE(boxColliderComponent->isEnabled());
    ASSERT_TRUE(transformComponent->isEnabled());
}

TEST_F(ComponentOwnerTest, givenEnabledComponents_shouldReturnTrue)
{
    componentOwner.addComponent<BoxColliderComponent>(utils::Vector2f{3, 0});

    ASSERT_TRUE(componentOwner.areComponentsEnabled());
}

TEST_F(ComponentOwnerTest, givenDisabledComponents_shouldReturnFalse)
{
    const auto boxColliderComponent =
        componentOwner.addComponent<BoxColliderComponent>(utils::Vector2f{3, 0});
    boxColliderComponent->disable();

    ASSERT_FALSE(componentOwner.areComponentsEnabled());
}

TEST_F(ComponentOwnerTest, defaultComponentOwner_shouldNotBeSetToRemoval)
{
    ASSERT_FALSE(componentOwner.shouldBeRemoved());
}

TEST_F(ComponentOwnerTest, givenRemovedComponentOwner_shouldReturnTrue)
{
    componentOwner.remove();
    ASSERT_TRUE(componentOwner.shouldBeRemoved());
}