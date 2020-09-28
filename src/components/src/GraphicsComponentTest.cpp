#include "GraphicsComponent.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "DeltaTime.h"
#include "GraphicsIdGenerator.h"

using namespace ::testing;
using namespace graphics;
using namespace components;

class GraphicsComponentTest : public Test
{
public:
    void expectCreateGraphicsComponent()
    {
        EXPECT_CALL(*rendererPool, acquire(size, position1, color, VisibilityLayer::First))
            .WillOnce(Return(graphicsId));
    }

    void expectReleaseGraphicsId()
    {
        EXPECT_CALL(*rendererPool, release(graphicsId));
    }

    std::shared_ptr<GraphicsComponent> createGraphicsComponent()
    {
        return std::make_shared<GraphicsComponent>(&componentOwner, rendererPool, size, position1, color);
    }

    std::shared_ptr<GraphicsComponent> createGraphicsComponentWithTexturePath()
    {
        return std::make_shared<GraphicsComponent>(&componentOwner, rendererPool, size, position1,
                                                   texturePath);
    }

    const utils::Vector2f size{0, 10};
    const utils::Vector2f position1{0, 10};
    const utils::Vector2f position2{12, 2};
    const Color color{Color::Red};
    const GraphicsId graphicsId = GraphicsIdGenerator::generateId();
    std::shared_ptr<StrictMock<RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<RendererPoolMock>>();
    ComponentOwner componentOwner{position1};
    utils::DeltaTime deltaTime{1};
    graphics::TexturePath texturePath{"/path/to/texture"};
};

TEST_F(GraphicsComponentTest, createGraphicsComponent_shouldCreateGraphicsShape)
{
    EXPECT_CALL(*rendererPool, acquire(size, position1, color, VisibilityLayer::First))
        .WillOnce(Return(graphicsId));

    const auto graphicsComponent = createGraphicsComponent();

    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, createGraphicsComponent_shouldCreateGraphicsShapeWithTexturePath)
{
    EXPECT_CALL(*rendererPool, acquire(size, position1, texturePath, VisibilityLayer::First))
        .WillOnce(Return(graphicsId));

    const auto graphicsComponent = createGraphicsComponentWithTexturePath();

    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, getGraphicsId_shouldReturnGraphicsIdCreatedWithConstructionOfComponent)
{
    expectCreateGraphicsComponent();
    const auto graphicsComponent = createGraphicsComponent();

    const auto actualGraphicsId = graphicsComponent->getGraphicsId();

    ASSERT_EQ(actualGraphicsId, graphicsId);
    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, lateUpdate_shouldSynchronizePositionWithTransformComponent)
{
    expectCreateGraphicsComponent();
    const auto graphicsComponent = createGraphicsComponent();
    componentOwner.transform->setPosition(position2);
    EXPECT_CALL(*rendererPool, setPosition(graphicsId, position2));

    graphicsComponent->lateUpdate(deltaTime);

    expectReleaseGraphicsId();
}