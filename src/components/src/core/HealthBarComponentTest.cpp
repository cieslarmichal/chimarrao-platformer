#include "HealthBarComponent.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "GraphicsIdGenerator.h"
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

    const utils::Vector2f size{3.f, 0.5f};
    const graphics::Color color{graphics::Color::Red};
    const graphics::VisibilityLayer initialVisibility{graphics::VisibilityLayer::First};
    const utils::DeltaTime deltaTime{3};
    const utils::Vector2f position{50.0, 11.0};
    const graphics::GraphicsId graphicsId = graphics::GraphicsIdGenerator::generateId();
    StrictMock<input::InputMock> input;
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
};

TEST_F(HealthBarComponentTest,
       loadDependentComponentsWithoutHealthComponent_shouldThrowDependentComponentNotFound)
{
    expectCreateGraphicsComponent();
    ComponentOwner componentOwnerWithoutHealth{position, "componentOwnerWithoutHealth"};
    HealthBarComponent healthBarComponentWithoutHealth{&componentOwnerWithoutHealth, rendererPool};

    ASSERT_THROW(healthBarComponentWithoutHealth.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
    expectReleaseGraphicsId();
}
