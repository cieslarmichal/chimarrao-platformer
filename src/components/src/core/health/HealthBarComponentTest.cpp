#include "HealthBarComponent.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "GraphicsIdGenerator.h"
#include "HealthComponent.h"
#include "exceptions/DependentComponentNotFound.h"

using namespace components::core;
using namespace ::testing;

class HealthBarComponentTest : public Test
{
public:
    void expectCreateGraphicsComponent()
    {
        EXPECT_CALL(*rendererPool, acquire(size, position, color, initialVisibility, _))
            .WillOnce(Return(graphicsId));
    }

    void expectReleaseGraphicsId()
    {
        EXPECT_CALL(*rendererPool, release(graphicsId));
    }

    const utils::Vector2f size{2.5f, 0.4f};
    const utils::Vector2f sizeAfterLostHealth{1.25f, 0.4f};
    const graphics::Color color{graphics::Color::Red};
    const graphics::VisibilityLayer initialVisibility{graphics::VisibilityLayer::First};
    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{50.0, 11.0};
    const graphics::GraphicsId graphicsId = graphics::GraphicsIdGenerator::generateId();
    StrictMock<input::InputMock> input;
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position, "healthBarComponentTest", sharedContext};
};

TEST_F(HealthBarComponentTest,
       loadDependentComponentsWithoutHealthComponent_shouldThrowDependentComponentNotFound)
{
    expectCreateGraphicsComponent();
    ComponentOwner componentOwnerWithoutHealth{position, "componentOwnerWithoutHealth", sharedContext};
    HealthBarComponent healthBarComponentWithoutHealth{&componentOwnerWithoutHealth, rendererPool};

    ASSERT_THROW(healthBarComponentWithoutHealth.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
    expectReleaseGraphicsId();
}

TEST_F(HealthBarComponentTest, healthNotChanged_barShouldNotBeUpdated)
{
    expectCreateGraphicsComponent();
    HealthBarComponent healthBarComponent{&componentOwner, rendererPool};
    componentOwner.addComponent<HealthComponent>(100);
    healthBarComponent.loadDependentComponents();
    EXPECT_CALL(*rendererPool, getSize(graphicsId)).WillOnce(Return(size));
    EXPECT_CALL(*rendererPool, setPosition(graphicsId, _));

    healthBarComponent.update(deltaTime, input);
    healthBarComponent.lateUpdate(deltaTime, input);

    expectReleaseGraphicsId();
}

TEST_F(HealthBarComponentTest, healthChanged_barShouldBeUpdated)
{
    expectCreateGraphicsComponent();
    HealthBarComponent healthBarComponent{&componentOwner, rendererPool};
    auto health = componentOwner.addComponent<HealthComponent>(100);
    healthBarComponent.loadDependentComponents();
    health->loseHealthPoints(50);
    EXPECT_CALL(*rendererPool, getSize(graphicsId)).WillOnce(Return(size));
    EXPECT_CALL(*rendererPool, setSize(graphicsId, sizeAfterLostHealth));
    EXPECT_CALL(*rendererPool, setPosition(graphicsId, _));

    healthBarComponent.update(deltaTime, input);
    healthBarComponent.lateUpdate(deltaTime, input);

    expectReleaseGraphicsId();
}
