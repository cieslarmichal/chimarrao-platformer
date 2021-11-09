#include "GraphicsComponent.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "DeltaTime.h"
#include "GraphicsIdGenerator.h"

using namespace ::testing;
using namespace graphics;
using namespace components::core;

class GraphicsComponentTest : public Test
{
public:
    void expectCreateGraphicsComponent()
    {
        EXPECT_CALL(*rendererPool, acquire(size, position1, color1, initialVisibility, _))
            .WillOnce(Return(graphicsId));
    }

    void expectReleaseGraphicsId()
    {
        EXPECT_CALL(*rendererPool, release(graphicsId));
    }

    std::shared_ptr<GraphicsComponent> createGraphicsComponent()
    {
        return std::make_shared<GraphicsComponent>(&componentOwner, rendererPool, size, position1, color1);
    }

    std::shared_ptr<GraphicsComponent> createGraphicsComponentWithTexturePath()
    {
        return std::make_shared<GraphicsComponent>(&componentOwner, rendererPool, size, position1,
                                                   texturePath);
    }

    const utils::Vector2f size{0, 10};
    const utils::Vector2f size2{20, 20};
    const utils::Vector2f position1{0, 10};
    const utils::Vector2f position2{12, 2};
    const Color color1{Color::Red};
    const Color color2{Color::Green};
    const VisibilityLayer initialVisibility{VisibilityLayer::First};
    const VisibilityLayer visibility{VisibilityLayer::Second};
    const VisibilityLayer invisible{VisibilityLayer::Invisible};
    const GraphicsId graphicsId = GraphicsIdGenerator::generateId();
    std::shared_ptr<StrictMock<RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position1, "graphicsComponentTest", sharedContext};
    utils::DeltaTime deltaTime{1};
    graphics::TexturePath texturePath{"/path/to/texture"};
    input::InputMock input;
};

TEST_F(GraphicsComponentTest, createGraphicsComponent_shouldCreateGraphicsShape)
{
    EXPECT_CALL(*rendererPool, acquire(size, position1, color1, VisibilityLayer::First, _))
        .WillOnce(Return(graphicsId));

    const auto graphicsComponent = createGraphicsComponent();

    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, createGraphicsComponent_shouldCreateGraphicsShapeWithTexturePath)
{
    EXPECT_CALL(*rendererPool, acquire(size, position1, texturePath, VisibilityLayer::First, _))
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

    graphicsComponent->lateUpdate(deltaTime, input);

    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, componentDisabled_lateUpdate_shouldNotSynchronizePositionWithTransformComponent)
{
    expectCreateGraphicsComponent();
    const auto graphicsComponent = createGraphicsComponent();
    componentOwner.transform->setPosition(position2);
    EXPECT_CALL(*rendererPool, setVisibility(graphicsId, invisible));
    graphicsComponent->disable();

    graphicsComponent->lateUpdate(deltaTime, input);

    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, shouldSetColor)
{
    expectCreateGraphicsComponent();
    const auto graphicsComponent = createGraphicsComponent();
    EXPECT_CALL(*rendererPool, setColor(graphicsId, color2));

    graphicsComponent->setColor(color2);

    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, shouldSetVisibility)
{
    expectCreateGraphicsComponent();
    const auto graphicsComponent = createGraphicsComponent();
    EXPECT_CALL(*rendererPool, setVisibility(graphicsId, visibility));

    graphicsComponent->setVisibility(visibility);

    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, disableComponent_shouldSetInvisibleAndSetDisabled)
{
    expectCreateGraphicsComponent();
    const auto graphicsComponent = createGraphicsComponent();
    EXPECT_CALL(*rendererPool, setVisibility(graphicsId, invisible));

    graphicsComponent->disable();

    EXPECT_FALSE(graphicsComponent->isEnabled());
    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, enableDisabledComponent_shouldSetInitialVisibilityLayerAndSetEnabled)
{
    expectCreateGraphicsComponent();
    const auto graphicsComponent = createGraphicsComponent();
    EXPECT_CALL(*rendererPool, setVisibility(graphicsId, invisible));
    graphicsComponent->disable();

    EXPECT_CALL(*rendererPool, setVisibility(graphicsId, initialVisibility));
    graphicsComponent->enable();

    EXPECT_TRUE(graphicsComponent->isEnabled());
    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, shouldSetSize)
{
    expectCreateGraphicsComponent();
    const auto graphicsComponent = createGraphicsComponent();
    EXPECT_CALL(*rendererPool, setSize(graphicsId, size2));
    EXPECT_CALL(*rendererPool, getSize(graphicsId)).WillOnce(Return(size2));

    graphicsComponent->setSize(size2);

    ASSERT_EQ(graphicsComponent->getSize(), size2);
    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, graphicsComponentWithColor_shouldReturnNoneTexturePath)
{
    EXPECT_CALL(*rendererPool, acquire(size, position1, color1, VisibilityLayer::First, _))
        .WillOnce(Return(graphicsId));
    const auto graphicsComponent = createGraphicsComponent();

    const auto actualTexturePath = graphicsComponent->getTexturePath();

    ASSERT_EQ(actualTexturePath, boost::none);
    expectReleaseGraphicsId();
}

TEST_F(GraphicsComponentTest, graphicsComponentWithTexture_shouldReturnTexturePath)
{
    EXPECT_CALL(*rendererPool, acquire(size, position1, texturePath, VisibilityLayer::First, _))
        .WillOnce(Return(graphicsId));
    const auto graphicsComponent = createGraphicsComponentWithTexturePath();

    const auto actualTexturePath = graphicsComponent->getTexturePath();

    ASSERT_EQ(actualTexturePath, texturePath);
    expectReleaseGraphicsId();
}
