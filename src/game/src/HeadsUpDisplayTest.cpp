#include "HeadsUpDisplay.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "CommonUIConfigElements.h"
#include "GraphicsIdGenerator.h"
#include "HeadsUpDisplayUIConfigBuilder.h"
#include "core/HealthComponent.h"

using namespace components::core;
using namespace game;
using namespace ::testing;

class HeadsUpDisplayTest : public Test
{
public:
    void expectCreateGraphicsComponents()
    {
        EXPECT_CALL(*rendererPool, acquire(healthPointsBarSize, healthPointsBarPosition, graphics::Color::Red,
                                           graphics::VisibilityLayer::First, _))
            .WillOnce(Return(graphicsId1));
        EXPECT_CALL(*rendererPool, acquire(healthPointsBarSize, healthPointsBarPosition,
                                           graphics::Color::Transparent, graphics::VisibilityLayer::First, _))
            .WillOnce(Return(graphicsId3));
        EXPECT_CALL(*rendererPool, acquireText(healthPointsLabelPosition, "HP", fontPath, characterSize,
                                               graphics::VisibilityLayer::First, graphics::Color::Red, _))
            .WillOnce(Return(graphicsId2));
    }

    void expectReleaseGraphicsIds()
    {
        EXPECT_CALL(*rendererPool, release(graphicsId1));
        EXPECT_CALL(*rendererPool, release(graphicsId2));
        EXPECT_CALL(*rendererPool, release(graphicsId3));
    }

    const unsigned characterSize{20};
    const utils::Vector2f healthPointsLabelPosition{0.5, 0.5};
    const utils::Vector2f healthPointsBarPosition{4, 1.2};
    const utils::Vector2f healthPointsBarSize{7, 1.3};
    const utils::Vector2f healthPointsBarSizeAfterChange{3.5f, 1.3};
    const utils::Vector2f position{50.0, 11.0};
    const graphics::GraphicsId graphicsId1 = graphics::GraphicsIdGenerator::generateId();
    const graphics::GraphicsId graphicsId2 = graphics::GraphicsIdGenerator::generateId();
    const graphics::GraphicsId graphicsId3 = graphics::GraphicsIdGenerator::generateId();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    std::shared_ptr<ComponentOwner> player{std::make_shared<ComponentOwner>(position, "headsUpDisplayTest")};
};

TEST_F(HeadsUpDisplayTest, healthNotChanged_barShouldNotBeUpdated)
{
    expectCreateGraphicsComponents();
    player->addComponent<HealthComponent>(100);
    player->loadDependentComponents();
    EXPECT_CALL(*rendererPool, setOutline(graphicsId3, 0.15, graphics::Color::Black));
    auto hud = HeadsUpDisplay{player, rendererPool, HeadsUpDisplayUIConfigBuilder::createUIConfig()};
    EXPECT_CALL(*rendererPool, getSize(graphicsId1)).WillOnce(Return(healthPointsBarSize));

    hud.update();

    expectReleaseGraphicsIds();
}

TEST_F(HeadsUpDisplayTest, healthChanged_barShouldBeUpdated)
{
    expectCreateGraphicsComponents();
    auto health = player->addComponent<HealthComponent>(100);
    player->loadDependentComponents();
    health->loseHealthPoints(50);
    EXPECT_CALL(*rendererPool, setOutline(graphicsId3, 0.15, graphics::Color::Black));
    auto hud = HeadsUpDisplay{player, rendererPool, HeadsUpDisplayUIConfigBuilder::createUIConfig()};
    EXPECT_CALL(*rendererPool, getSize(graphicsId1)).WillOnce(Return(healthPointsBarSize));
    EXPECT_CALL(*rendererPool, setSize(graphicsId1, healthPointsBarSizeAfterChange));

    hud.update();

    expectReleaseGraphicsIds();
}