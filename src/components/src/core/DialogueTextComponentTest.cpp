#include "DialogueTextComponent.h"

#include "gtest/gtest.h"

#include "ComponentOwner.h"
#include "RendererPoolMock.h"

using namespace components::core;
using namespace graphics;
using namespace ::testing;

class DialogueTextComponentTest : public Test
{
public:
    std::shared_ptr<DialogueTextComponent> createDialogueTextComponent()
    {
        return std::make_shared<DialogueTextComponent>(&componentOwner, rendererPool, position1, text, fontPath,
                                               characterSize, color1);
    }

    const utils::Vector2f offset{10, 10};
    const utils::Vector2f position1{0, 10};
    const FontPath fontPath{"fontPath"};
    const std::string text{"text"};
    const std::string updatedText1{"wow"};
    const std::string updatedText2{"wowow"};
    const std::string updatedText3{"wowowowo"};
    const unsigned characterSize{15};
    const Color color1{Color::Black};
    const VisibilityLayer invisible{VisibilityLayer::Invisible};
    std::shared_ptr<NiceMock<RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner{position1, "textComponentTest", sharedContext};
    utils::DeltaTime deltaTime{1};
};

TEST_F(DialogueTextComponentTest, shouldSetTextInTextComponent)
{
    const auto dialogueTextComponent = createDialogueTextComponent();

    EXPECT_CALL(*rendererPool, setText(_, updatedText1));

    dialogueTextComponent->setText(updatedText1);
}

TEST_F(DialogueTextComponentTest, shouldSetOffsetInTextComponent)
{
    const auto dialogueTextComponent = createDialogueTextComponent();

    dialogueTextComponent->setText(updatedText1);
    ASSERT_EQ(componentOwner.getComponent<TextComponent>()->getOffset().x, 2.5f);

    dialogueTextComponent->setText(updatedText2);
    ASSERT_EQ(componentOwner.getComponent<TextComponent>()->getOffset().x, 2.f);

    dialogueTextComponent->setText(updatedText3);
    ASSERT_EQ(componentOwner.getComponent<TextComponent>()->getOffset().x, 1.5f);
}
