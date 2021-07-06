#include "Button.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"
#include "exceptions/UIComponentConfigNotFound.h"
#include "TimerMock.h"
#include "ProjectPathReader.h"
#include "InputMock.h"

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
const auto buttonName = "dummyButton";
const auto deltaTime = utils::DeltaTime{1};

std::unique_ptr<components::ui::ButtonConfig> createValidConfig()
{
    const auto dummyButtonMouseOver = [=] {};
    const auto dummyButtonMouseOut = [=] {};
    auto dummyButtonMouseOverActions =
        components::ui::MouseOverActions{dummyButtonMouseOver, dummyButtonMouseOut};
    auto dummyButtonClickAction = [=] {};
    return std::make_unique<components::ui::ButtonConfig>(
        buttonName, dummyPosition, dummySize, dummyColor, dummyText, dummyColor, dummyFontSize, fontPath,
        utils::Vector2f{2.f, 0.f}, dummyButtonClickAction, dummyButtonMouseOverActions);
}
}

class ButtonTest : public Test
{
public:
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::unique_ptr<NiceMock<utils::TimerMock>> timerInit =
        std::make_unique<NiceMock<utils::TimerMock>>();
    NiceMock<utils::TimerMock>* timer = timerInit.get();
    NiceMock<input::InputMock> input;
};

TEST_F(ButtonTest, createBackgroundWithoutConfig_shouldThrowUIComponentConfigNotFound)
{
    ASSERT_THROW(Button(rendererPool, nullptr, std::move(timerInit)), exceptions::UIComponentConfigNotFound);
}

TEST_F(ButtonTest, createBackgroundWithValidConfig_shouldNoThrow)
{
    ASSERT_NO_THROW(Button(rendererPool, createValidConfig(), std::move(timerInit)));
}

TEST_F(ButtonTest, activate_shouldActivateAfterSomeTime)
{
    auto button = Button(rendererPool, createValidConfig(), std::move(timerInit));

    button.activate();
    ASSERT_FALSE(button.isActive());

    EXPECT_CALL(*timer, getElapsedSeconds()).WillOnce(Return(1));
    button.update(deltaTime, input);
    ASSERT_TRUE(button.isActive());
}

TEST_F(ButtonTest, deactivate)
{
    auto button = Button(rendererPool, createValidConfig(), std::move(timerInit));

    button.deactivate();

    ASSERT_FALSE(button.isActive());
}

TEST_F(ButtonTest, getName)
{
    auto button = Button(rendererPool, createValidConfig(), std::move(timerInit));

    const auto actualName = button.getName();

    ASSERT_EQ(actualName, buttonName);
}

TEST_F(ButtonTest, setColor)
{
    auto button = Button(rendererPool, createValidConfig(), std::move(timerInit));
    EXPECT_CALL(*rendererPool, setColor(_, dummyColor));

    button.setColor(dummyColor);
}

TEST_F(ButtonTest, setText)
{
    auto button = Button(rendererPool, createValidConfig(), std::move(timerInit));
    EXPECT_CALL(*rendererPool, setText(_, dummyText2));

    button.setText(dummyText2);

    EXPECT_CALL(*rendererPool, getText(_)).WillOnce(Return(textFromRendererPool));
    ASSERT_EQ(button.getText(), dummyText2);
}