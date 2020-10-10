#include "ComponentOwner.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"

#include "AnimationComponent.h"

using namespace ::testing;
using namespace components;
using namespace animations;

class ComponentOwnerTest : public Test
{
public:
    const utils::Vector2f initialPosition{0.0, 11.0};
    std::shared_ptr<StrictMock<AnimatorMock>> animator = std::make_shared<StrictMock<AnimatorMock>>();

    ComponentOwner componentOwner{initialPosition};
};

TEST_F(ComponentOwnerTest, initialPosition_shouldBeSetFromConstructor)
{
    const auto actualPosition = componentOwner.transform->getPosition();

    ASSERT_EQ(actualPosition, initialPosition);
}

TEST_F(ComponentOwnerTest, addComponent_shouldBeValid)
{
    const auto animationComponent = componentOwner.addComponent<AnimationComponent>(animator);

    ASSERT_TRUE(animationComponent);
}

TEST_F(ComponentOwnerTest, addComponentTwoTimes_shouldBeReturnSameComponentPointer)
{
    const auto animationComponent1 = componentOwner.addComponent<AnimationComponent>(animator);
    const auto animationComponent2 = componentOwner.addComponent<AnimationComponent>(animator);

    ASSERT_EQ(animationComponent1, animationComponent2);
}

TEST_F(ComponentOwnerTest, getComponentWhenComponentNotCreated_shouldReturnNullptr)
{
    const auto animationComponent = componentOwner.getComponent<AnimationComponent>();

    ASSERT_FALSE(animationComponent);
}

TEST_F(ComponentOwnerTest, getComponentWhenComponentCreated_shouldReturnValidComponentPointer)
{
    const auto addedAnimationComponent = componentOwner.addComponent<AnimationComponent>(animator);

    const auto animationComponentByGet = componentOwner.getComponent<AnimationComponent>();

    ASSERT_EQ(addedAnimationComponent, animationComponentByGet);
}

TEST_F(ComponentOwnerTest, shouldDisableComponents)
{
    auto animationComponent = componentOwner.addComponent<AnimationComponent>(animator);
    auto transformComponent = componentOwner.transform;

    componentOwner.disable();

    ASSERT_FALSE(animationComponent->isEnabled());
    ASSERT_FALSE(transformComponent->isEnabled());
}

TEST_F(ComponentOwnerTest, shouldEnableComponents)
{
    auto animationComponent = componentOwner.addComponent<AnimationComponent>(animator);
    auto transformComponent = componentOwner.transform;
    componentOwner.disable();

    componentOwner.enable();

    ASSERT_TRUE(animationComponent->isEnabled());
    ASSERT_TRUE(transformComponent->isEnabled());
}