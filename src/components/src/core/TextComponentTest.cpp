#include "TextComponent.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "DeltaTime.h"
#include "GraphicsIdGenerator.h"

using namespace components::core;
using namespace graphics;
using namespace ::testing;

class TextComponentTest : public Test
{
public:
    void expectCreateTextComponent()
    {
        EXPECT_CALL(*rendererPool,
                    acquireText(position1, text, fontPath, characterSize, initialVisibility, color1, _))
            .WillOnce(Return(graphicsId));
    }

    void expectReleaseGraphicsId()
    {
        EXPECT_CALL(*rendererPool, release(graphicsId));
    }

    std::shared_ptr<TextComponent> createTextComponent()
    {
        return std::make_shared<TextComponent>(&componentOwner, rendererPool, position1, text, fontPath,
                                               characterSize, color1);
    }

    std::shared_ptr<TextComponent> createTextComponentWithOffset()
    {
        return std::make_shared<TextComponent>(&componentOwner, rendererPool, position1, text, fontPath,
                                               characterSize, color1, offset);
    }

    const utils::Vector2f offset{10, 10};
    const utils::Vector2f position1{0, 10};
    const utils::Vector2f position2{12, 2};
    const FontPath fontPath{"fontPath"};
    const std::string text{"text"};
    boost::optional<std::string> textFromRendererPool{text};
    const std::string updatedText{"wowow"};
    const unsigned characterSize{15};
    const Color color1{Color::Black};
    const Color color2{Color::White};
    const VisibilityLayer initialVisibility{VisibilityLayer::First};
    const VisibilityLayer invisible{VisibilityLayer::Invisible};
    const GraphicsId graphicsId{GraphicsIdGenerator::generateId()};
    std::shared_ptr<StrictMock<RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position1, "textComponentTest", sharedContext};
    utils::DeltaTime deltaTime{1};
    input::InputMock input;
};

TEST_F(TextComponentTest, createTextComponent_shouldCreateGraphicsObject)
{
    EXPECT_CALL(*rendererPool,
                acquireText(position1, text, fontPath, characterSize, initialVisibility, color1, _))
        .WillOnce(Return(graphicsId));

    const auto textComponent = createTextComponent();

    expectReleaseGraphicsId();
}

TEST_F(TextComponentTest, getGraphicsId_shouldReturnGraphicsIdCreatedWithConstructionOfComponent)
{
    expectCreateTextComponent();
    const auto textComponent = createTextComponent();

    const auto actualGraphicsId = textComponent->getGraphicsId();

    ASSERT_EQ(actualGraphicsId, graphicsId);
    expectReleaseGraphicsId();
}

TEST_F(TextComponentTest,
       componentWithoutTransformOffset_lateUpdate_shouldSynchronizePositionWithTransformComponent)
{
    expectCreateTextComponent();
    const auto textComponent = createTextComponent();
    componentOwner.transform->setPosition(position2);
    EXPECT_CALL(*rendererPool, setPosition(graphicsId, position2));

    textComponent->lateUpdate(deltaTime, input);

    expectReleaseGraphicsId();
}

TEST_F(TextComponentTest, componentDisabled_lateUpdate_shouldNotSynchronizePositionWithTransformComponent)
{
    expectCreateTextComponent();
    const auto textComponent = createTextComponent();
    componentOwner.transform->setPosition(position2);
    EXPECT_CALL(*rendererPool, setVisibility(graphicsId, invisible));
    textComponent->disable();

    textComponent->lateUpdate(deltaTime, input);

    expectReleaseGraphicsId();
}

TEST_F(
    TextComponentTest,
    componentWithTransformOffset_lateUpdate_shouldSynchronizePositionWithTransformComponentConsideringTransformOffset)
{
    expectCreateTextComponent();
    const auto textComponent = createTextComponentWithOffset();
    componentOwner.transform->setPosition(position2);
    EXPECT_CALL(*rendererPool, setPosition(graphicsId, position2 + offset));

    textComponent->lateUpdate(deltaTime, input);

    expectReleaseGraphicsId();
}

TEST_F(
    TextComponentTest,
    componentCreatedWithoutTransformOffsetAndSetOffsetLater_lateUpdate_shouldSynchronizePositionWithTransformComponentConsideringTransformOffset)
{
    expectCreateTextComponent();
    const auto textComponent = createTextComponent();
    componentOwner.transform->setPosition(position2);
    EXPECT_CALL(*rendererPool, setPosition(graphicsId, position2 + offset));

    textComponent->setOffset(offset);

    textComponent->lateUpdate(deltaTime, input);
    expectReleaseGraphicsId();
}

TEST_F(TextComponentTest, disableComponent_shouldSetInvisibleAndSetDisabled)
{
    expectCreateTextComponent();
    const auto textComponent = createTextComponent();
    EXPECT_CALL(*rendererPool, setVisibility(graphicsId, invisible));

    textComponent->disable();

    EXPECT_FALSE(textComponent->isEnabled());
    expectReleaseGraphicsId();
}

TEST_F(TextComponentTest, enableDisabledComponent_shouldSetInitialVisibilityLayerAndSetEnabled)
{
    expectCreateTextComponent();
    const auto textComponent = createTextComponent();
    EXPECT_CALL(*rendererPool, setVisibility(graphicsId, invisible));
    textComponent->disable();

    EXPECT_CALL(*rendererPool, setVisibility(graphicsId, initialVisibility));
    textComponent->enable();

    EXPECT_TRUE(textComponent->isEnabled());
    expectReleaseGraphicsId();
}

TEST_F(TextComponentTest, shouldSetColor)
{
    expectCreateTextComponent();
    const auto textComponent = createTextComponent();

    EXPECT_CALL(*rendererPool, setColor(textComponent->getGraphicsId(), color2));

    textComponent->setColor(color2);
    expectReleaseGraphicsId();
}

TEST_F(TextComponentTest, shouldSetText)
{
    expectCreateTextComponent();
    const auto textComponent = createTextComponent();

    EXPECT_CALL(*rendererPool, setText(textComponent->getGraphicsId(), updatedText));

    textComponent->setText(updatedText);
    expectReleaseGraphicsId();
}

TEST_F(TextComponentTest, getText)
{
    expectCreateTextComponent();
    const auto textComponent = createTextComponent();
    EXPECT_CALL(*rendererPool, getText(textComponent->getGraphicsId()))
        .WillOnce(Return(textFromRendererPool));

    const auto actualText = textComponent->getText();

    ASSERT_EQ(actualText, text);
    expectReleaseGraphicsId();
}
