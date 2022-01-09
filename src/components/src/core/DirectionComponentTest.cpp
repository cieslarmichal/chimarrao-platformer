#include "DirectionComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "RendererPoolMock.h"

#include "exceptions/DependentComponentNotFound.h"

using namespace ::testing;
using namespace components::core;

class DirectionComponentTest : public Test
{
public:
    DirectionComponentTest()
    {
        velocityComponent = componentOwner.addComponent<VelocityComponent>(6);
        componentOwner.addComponent<AnimationComponent>(animator);
        directionComponent.loadDependentComponents();
    }

    const float x{1.0};
    const float y{2.0};
    const utils::Vector2f position1{0.0, 10.0};
    const utils::Vector2f velocityDirectedRight{10.0, 0};
    const utils::Vector2f velocityDirectedLeft{-10.0, 0};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position1, "DirectionComponentTest", sharedContext};
    std::shared_ptr<VelocityComponent> velocityComponent;
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    DirectionComponent directionComponent{&componentOwner};
};

TEST_F(DirectionComponentTest,
       loadDependentComponentsWithoutAnimationComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutAnimation{position1, "componentOwnerWithoutAnimation", sharedContext};
    DirectionComponent directionComponentWithoutAnimation{&componentOwnerWithoutAnimation};

    ASSERT_THROW(directionComponentWithoutAnimation.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(DirectionComponentTest,
       loadDependentComponentsWithoutVelocityComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutVelocity{position1, "componentOwnerWithoutVelocity", sharedContext};
    DirectionComponent directionComponentWithoutVelocity{&componentOwnerWithoutVelocity};
    componentOwnerWithoutVelocity.addComponent<AnimationComponent>(animator);

    ASSERT_THROW(directionComponentWithoutVelocity.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(DirectionComponentTest, givenVelocityGoingRight_shouldReturnRightDirection)
{
    velocityComponent->setVelocity(velocityDirectedRight);

    ASSERT_EQ(directionComponent.getDirection(), animations::AnimationDirection::Right);
}

TEST_F(DirectionComponentTest, givenVelocityGoingLeft_shouldReturnLeftDirection)
{
    velocityComponent->setVelocity(velocityDirectedLeft);

    ASSERT_EQ(directionComponent.getDirection(), animations::AnimationDirection::Left);
}

TEST_F(DirectionComponentTest, givenVelocityGoingRight_shouldReturnPositiveXHeading)
{
    velocityComponent->setVelocity(velocityDirectedRight);
    const auto expectedHeading = utils::Vector2i{1, 1};

    const auto actualHeading = directionComponent.getHeading();

    ASSERT_EQ(actualHeading, expectedHeading);
}

TEST_F(DirectionComponentTest, givenVelocityGoingLeft_shouldReturnPositiveXHeading)
{
    velocityComponent->setVelocity(velocityDirectedLeft);
    const auto expectedHeading = utils::Vector2i{-1, 1};

    const auto actualHeading = directionComponent.getHeading();

    ASSERT_EQ(actualHeading, expectedHeading);
}