#include "ComponentOwner.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "BoxColliderComponent.h"
#include "GraphicsIdGenerator.h"
#include "ProjectPathReader.h"

using namespace ::testing;
using namespace components::core;

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto dummySize = utils::Vector2f{25.f, 5.f};
const auto changedSize = utils::Vector2f{10.f, 5.f};
const auto dummyPosition = utils::Vector2f{0.f, 5.f};
const auto imagePath = projectPath + "resources/BG/menu_background.jpg";
}

class ComponentOwnerTest : public Test
{
public:
    const utils::Vector2f initialPosition{0.0, 11.0};
    const graphics::GraphicsId graphicsId1 = graphics::GraphicsIdGenerator::generateId();
    const graphics::GraphicsId graphicsId2 = graphics::GraphicsIdGenerator::generateId();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
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

TEST_F(ComponentOwnerTest, graphicsComponentAddedAsFirst_shouldBeMainGraphicsComponent)
{
    EXPECT_CALL(*rendererPool,
                acquire(dummySize, dummyPosition, imagePath, graphics::VisibilityLayer::First, _))
        .WillOnce(Return(graphicsId1));
    EXPECT_CALL(*rendererPool, acquire(dummySize, dummySize, imagePath, graphics::VisibilityLayer::First, _))
        .WillOnce(Return(graphicsId2));
    const auto expectedMainGraphicsComponent =
        componentOwner.addGraphicsComponent(rendererPool, dummySize, dummyPosition, imagePath,
                                            graphics::VisibilityLayer::First, utils::Vector2f{0, 0}, true);
    componentOwner.addGraphicsComponent(rendererPool, dummySize, dummySize, imagePath,
                                        graphics::VisibilityLayer::First, utils::Vector2f{0, 0}, true);

    const auto actualMainGraphicsComponent = componentOwner.getMainGraphicsComponent();

    ASSERT_EQ(expectedMainGraphicsComponent.get(), actualMainGraphicsComponent.get());
    EXPECT_CALL(*rendererPool, release(graphicsId1));
    EXPECT_CALL(*rendererPool, release(graphicsId2));
}

TEST_F(ComponentOwnerTest, allAddedGraphicsComponentsShouldBeStored)
{
    EXPECT_CALL(*rendererPool,
                acquire(dummySize, dummyPosition, imagePath, graphics::VisibilityLayer::First, _))
        .WillOnce(Return(graphicsId1));
    EXPECT_CALL(*rendererPool, acquire(dummySize, dummySize, imagePath, graphics::VisibilityLayer::First, _))
        .WillOnce(Return(graphicsId2));
    const auto firstGraphicsComponent =
        componentOwner.addGraphicsComponent(rendererPool, dummySize, dummyPosition, imagePath,
                                            graphics::VisibilityLayer::First, utils::Vector2f{0, 0}, true);
    const auto secondGraphicsComponent =
        componentOwner.addGraphicsComponent(rendererPool, dummySize, dummySize, imagePath,
                                            graphics::VisibilityLayer::First, utils::Vector2f{0, 0}, true);

    const auto allGraphicsComponents = componentOwner.allGraphics;

    ASSERT_EQ(allGraphicsComponents.size(), 2);
    ASSERT_EQ(allGraphicsComponents.at(0).get(), firstGraphicsComponent.get());
    ASSERT_EQ(allGraphicsComponents.at(1).get(), secondGraphicsComponent.get());
    EXPECT_CALL(*rendererPool, release(graphicsId1));
    EXPECT_CALL(*rendererPool, release(graphicsId2));
}