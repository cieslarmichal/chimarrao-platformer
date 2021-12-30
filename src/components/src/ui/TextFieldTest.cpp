#include "TextField.h"

#include <algorithm>

#include "gtest/gtest.h"

#include "InputMock.h"
#include "RendererPoolMock.h"
#include "TimerMock.h"

#include "ProjectPathReader.h"
#include "exceptions/UIComponentConfigNotFound.h"

using namespace components::ui;
using namespace ::testing;

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto dummyColor = graphics::Color(251, 190, 102);
const auto dummySize = utils::Vector2f{25.f, 5.f};
const auto dummyPosition = utils::Vector2f{25.f, 5.f};
const auto dummyFontSize = 4;
const auto dummyText = "dummyText";
const auto dummyText2 = "dummyText2";
boost::optional<std::string> textFromRendererPool{dummyText2};
const auto fontPath = projectPath + "resources/fonts/VeraMono.ttf";
const auto textFieldName = "dummyTextField";
const auto deltaTime = utils::DeltaTime{1};

std::unique_ptr<components::ui::TextFieldConfig> createValidConfig()
{
    const auto dummyTextFieldOnMouseOver = [=] {};
    const auto dummyTextFieldOnMouseOut = [=] {};
    const auto dummyTextFieldMouseOverActions =
        components::ui::MouseOverActions{dummyTextFieldOnMouseOver, dummyTextFieldOnMouseOut};
    const auto dummyTextFieldClickInsideAction = [=] {};
    auto dummyTextFieldClickOutsideAction = [=]() {};
    return std::make_unique<components::ui::TextFieldConfig>(
        textFieldName, dummyPosition, dummySize, dummyColor, dummyText, dummyFontSize, fontPath,
        utils::Vector2f{0.75f, 0.4f}, dummyTextFieldClickInsideAction, dummyTextFieldClickOutsideAction,
        dummyTextFieldMouseOverActions);
}
}

class TextFieldTest : public Test
{
public:
    void expectReadAllAlphanumericalButtonsAsNonPressedExceptOfGivenButtons(
        const std::vector<input::InputKey>& pressedButtons)
    {
        const std::vector<input::InputKey> alphaNumericalButtons{
            input::InputKey::Number0, input::InputKey::Number1, input::InputKey::Number2,
            input::InputKey::Number3, input::InputKey::Number4, input::InputKey::Number5,
            input::InputKey::Number6, input::InputKey::Number7, input::InputKey::Number8,
            input::InputKey::Number9, input::InputKey::Q,       input::InputKey::W,
            input::InputKey::E,       input::InputKey::R,       input::InputKey::T,
            input::InputKey::Y,       input::InputKey::U,       input::InputKey::I,
            input::InputKey::O,       input::InputKey::P,       input::InputKey::A,
            input::InputKey::S,       input::InputKey::D,       input::InputKey::F,
            input::InputKey::G,       input::InputKey::H,       input::InputKey::J,
            input::InputKey::K,       input::InputKey::L,       input::InputKey::Z,
            input::InputKey::X,       input::InputKey::C,       input::InputKey::V,
            input::InputKey::B,       input::InputKey::N,       input::InputKey::M};

        for (const auto& button : alphaNumericalButtons)
        {
            if (std::any_of(pressedButtons.begin(), pressedButtons.end(),
                            [&](input::InputKey pressedKey) { return button == pressedKey; }))
            {
                EXPECT_CALL(input, isKeyReleased(button)).WillOnce(Return(true));
            }
            else
            {
                EXPECT_CALL(input, isKeyReleased(button)).WillOnce(Return(false));
            }
        }
    }
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    std::unique_ptr<NiceMock<utils::TimerMock>> timerInit = std::make_unique<NiceMock<utils::TimerMock>>();
    NiceMock<utils::TimerMock>* timer = timerInit.get();
    StrictMock<input::InputMock> input;
};

TEST_F(TextFieldTest, createBackgroundWithoutConfig_shouldThrowUIComponentConfigNotFound)
{
    ASSERT_THROW(TextField(sharedContext, nullptr, std::move(timerInit)),
                 exceptions::UIComponentConfigNotFound);
}

TEST_F(TextFieldTest, createBackgroundWithValidConfig_shouldNoThrow)
{
    ASSERT_NO_THROW(TextField(sharedContext, createValidConfig(), std::move(timerInit)));
}

TEST_F(TextFieldTest, activate_shouldActivateAfterSomeTime)
{
    auto textField = TextField(sharedContext, createValidConfig(), std::move(timerInit));

    textField.activate();

    ASSERT_TRUE(textField.isActive());
}

TEST_F(TextFieldTest, deactivate)
{
    auto textField = TextField(sharedContext, createValidConfig(), std::move(timerInit));

    textField.deactivate();

    ASSERT_FALSE(textField.isActive());
}

TEST_F(TextFieldTest, getName)
{
    auto textField = TextField(sharedContext, createValidConfig(), std::move(timerInit));

    const auto actualName = textField.getName();

    ASSERT_EQ(actualName, textFieldName);
}

TEST_F(TextFieldTest, setColor)
{
    auto textField = TextField(sharedContext, createValidConfig(), std::move(timerInit));
    EXPECT_CALL(*rendererPool, setColor(_, dummyColor));

    textField.setColor(dummyColor);
}

TEST_F(TextFieldTest, setText)
{
    auto textField = TextField(sharedContext, createValidConfig(), std::move(timerInit));
    EXPECT_CALL(*rendererPool, setText(_, dummyText2));

    textField.setText(dummyText2);

    ASSERT_EQ(textField.getText(), dummyText2);
}

TEST_F(TextFieldTest, update_withKeyboardCharactersInput_shouldAppendThemToText)
{
    auto textField = TextField(sharedContext, createValidConfig(), std::move(timerInit));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::MouseLeft)).WillOnce(Return(true));
    EXPECT_CALL(input, getMouseRelativePosition()).WillRepeatedly(Return(utils::Vector2f{26, 6}));
    expectReadAllAlphanumericalButtonsAsNonPressedExceptOfGivenButtons(
        {input::InputKey::A, input::InputKey::B});
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Backspace)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyReleased(input::InputKey::MouseLeft)).WillOnce(Return(true));

    textField.update(deltaTime, input);

    ASSERT_EQ(textField.getText(), "ab");
}

TEST_F(TextFieldTest, update_withKeyboardCharactersInputAndDeleteKeypress_shouldAppendThemToTextAndDelete)
{
    auto textField = TextField(sharedContext, createValidConfig(), std::move(timerInit));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::MouseLeft)).WillOnce(Return(true));
    EXPECT_CALL(input, getMouseRelativePosition()).WillRepeatedly(Return(utils::Vector2f{26, 6}));
    expectReadAllAlphanumericalButtonsAsNonPressedExceptOfGivenButtons(
        {input::InputKey::A, input::InputKey::B});
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Backspace)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyReleased(input::InputKey::MouseLeft)).WillOnce(Return(true));
    EXPECT_CALL(*timer, getElapsedSeconds()).WillOnce(Return(1));

    textField.update(deltaTime, input);

    ASSERT_EQ(textField.getText(), "a");
}