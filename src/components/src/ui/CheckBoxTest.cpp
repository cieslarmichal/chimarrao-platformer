#include "CheckBox.h"

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
const auto checkBoxName = "dummyCheckBox";
const auto deltaTime = utils::DeltaTime{1};

std::unique_ptr<components::ui::CheckBoxConfig> createValidConfig()
{
    const auto dummyCheckBoxOnMouseOver = [=] {};
    const auto dummyCheckBoxOnMouseOut = [=] {};
    auto dummyCheckBoxMouseOverActions =
        components::ui::MouseOverActions{dummyCheckBoxOnMouseOver, dummyCheckBoxOnMouseOut};
    auto dummyCheckBoxClickAction = [=] {};
    return std::make_unique<components::ui::CheckBoxConfig>(
        checkBoxName, dummyPosition, dummySize, dummyColor, false, dummyFontSize, fontPath,
        utils::Vector2f{0.8f, 0.0f}, dummyCheckBoxClickAction, dummyCheckBoxMouseOverActions);
}
}

class CheckBoxTest : public Test
{
public:
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    std::unique_ptr<NiceMock<utils::TimerMock>> timerInit = std::make_unique<NiceMock<utils::TimerMock>>();
    NiceMock<utils::TimerMock>* timer = timerInit.get();
    NiceMock<input::InputMock> input;
};

TEST_F(CheckBoxTest, createBackgroundWithoutConfig_shouldThrowUIComponentConfigNotFound)
{
    ASSERT_THROW(CheckBox(sharedContext, nullptr, std::move(timerInit)),
                 exceptions::UIComponentConfigNotFound);
}

TEST_F(CheckBoxTest, createBackgroundWithValidConfig_shouldNoThrow)
{
    ASSERT_NO_THROW(CheckBox(sharedContext, createValidConfig(), std::move(timerInit)));
}

TEST_F(CheckBoxTest, activate_shouldActivateAfterSomeTime)
{
    auto checkBox = CheckBox(sharedContext, createValidConfig(), std::move(timerInit));

    checkBox.activate();
    ASSERT_FALSE(checkBox.isActive());

    EXPECT_CALL(*timer, getElapsedSeconds()).WillOnce(Return(1));
    checkBox.update(deltaTime, input);
    ASSERT_TRUE(checkBox.isActive());
}

TEST_F(CheckBoxTest, deactivate)
{
    auto checkBox = CheckBox(sharedContext, createValidConfig(), std::move(timerInit));

    checkBox.deactivate();

    ASSERT_FALSE(checkBox.isActive());
}

TEST_F(CheckBoxTest, getName)
{
    auto checkBox = CheckBox(sharedContext, createValidConfig(), std::move(timerInit));

    const auto actualName = checkBox.getName();

    ASSERT_EQ(actualName, checkBoxName);
}

TEST_F(CheckBoxTest, setColor)
{
    auto checkBox = CheckBox(sharedContext, createValidConfig(), std::move(timerInit));
    EXPECT_CALL(*rendererPool, setColor(_, dummyColor));

    checkBox.setColor(dummyColor);
}

TEST_F(CheckBoxTest, setChecked)
{
    auto checkBox = CheckBox(sharedContext, createValidConfig(), std::move(timerInit));
    EXPECT_CALL(*rendererPool, setText(_, "X"));

    checkBox.setChecked(true);
}

TEST_F(CheckBoxTest, toggle)
{
    auto checkBox = CheckBox(sharedContext, createValidConfig(), std::move(timerInit));

    EXPECT_CALL(*rendererPool, setText(_, "X"));
    checkBox.toggle();
    EXPECT_CALL(*rendererPool, setText(_, ""));
    checkBox.toggle();
}