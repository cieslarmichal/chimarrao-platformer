#include "TextComponent.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "DeltaTime.h"
#include "GraphicsIdGenerator.h"

using namespace components;
using namespace graphics;
using namespace ::testing;

class TextComponentTest : public Test
{
public:
    void expectCreateTextComponent()
    {
        EXPECT_CALL(*rendererPool, acquireText(position1, text, fontPath,
                                               characterSize, color)).WillOnce(Return(graphicsId));
    }

    std::shared_ptr<TextComponent> createTextComponent()
    {
        return std::make_shared<TextComponent>(&componentOwner, rendererPool, position1, text, fontPath,
                                               characterSize, color);
    }

    std::shared_ptr<TextComponent> createTextComponentWithOffset()
    {
        return std::make_shared<TextComponent>(&componentOwner, rendererPool, position1, text, fontPath,
                                               characterSize, color, offset);
    }

    const utils::Vector2f offset{10, 10};
    const utils::Vector2f position1{0, 10};
    const utils::Vector2f position2{12, 2};
    const FontPath fontPath{"fontPath"};
    const std::string text{"text"};
    const unsigned characterSize{15};
    const Color color{Color::Black};
    const GraphicsId graphicsId{GraphicsIdGenerator::generateId()};
    std::shared_ptr<StrictMock<RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<RendererPoolMock>>();
    ComponentOwner componentOwner{position1};
    utils::DeltaTime deltaTime{1};
};

TEST_F(TextComponentTest, createTextComponent_shouldCreateGraphicsObject)
{
    EXPECT_CALL(*rendererPool, acquireText(position1, text, fontPath,
                                           characterSize, color)).WillOnce(Return(graphicsId));

    createTextComponent();
}

TEST_F(TextComponentTest, getGraphicsId_shouldReturnGraphicsIdCreatedWithConstructionOfComponent)
{
    expectCreateTextComponent();
    auto textComponent = createTextComponent();

    const auto actualGraphicsId = textComponent->getGraphicsId();

    ASSERT_EQ(actualGraphicsId, graphicsId);
}

TEST_F(TextComponentTest, componentWithoutTransformOffset_lateUpdate_shouldSynchronizePositionWithTransformComponent)
{
    expectCreateTextComponent();
    auto textComponent = createTextComponent();
    componentOwner.transform->setPosition(position2);
    EXPECT_CALL(*rendererPool, setPosition(graphicsId, position2));

    textComponent->lateUpdate(deltaTime);
}

TEST_F(TextComponentTest, componentWithTransformOffset_lateUpdate_shouldSynchronizePositionWithTransformComponentConsideringTransformOffset)
{
    expectCreateTextComponent();
    auto textComponent = createTextComponentWithOffset();
    componentOwner.transform->setPosition(position2);
    EXPECT_CALL(*rendererPool, setPosition(graphicsId, position2 + offset));

    textComponent->lateUpdate(deltaTime);
}
