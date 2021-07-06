#include "Label.h"

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
const boost::optional<std::string> textFromRendererPool{dummyText2};
const auto fontPath = projectPath + "resources/fonts/VeraMono.ttf";
const auto labelName = "dummyLabel";

std::unique_ptr<components::ui::LabelConfig> createValidConfig()
{
    return std::make_unique<components::ui::LabelConfig>(
        labelName, dummyPosition, dummyColor, dummyText, dummyFontSize, fontPath);
}
}

class LabelTest : public Test
{
public:
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
};

TEST_F(LabelTest, createBackgroundWithoutConfig_shouldThrowUIComponentConfigNotFound)
{
    ASSERT_THROW(Label(rendererPool, nullptr), exceptions::UIComponentConfigNotFound);
}

TEST_F(LabelTest, createBackgroundWithValidConfig_shouldNoThrow)
{
    ASSERT_NO_THROW(Label(rendererPool, createValidConfig()));
}

TEST_F(LabelTest, activate_shouldActivateAfterSomeTime)
{
    auto label = Label(rendererPool, createValidConfig());

    label.activate();

    ASSERT_TRUE(label.isActive());
}

TEST_F(LabelTest, deactivate)
{
    auto label = Label(rendererPool, createValidConfig());

    label.deactivate();

    ASSERT_FALSE(label.isActive());
}

TEST_F(LabelTest, getName)
{
    auto label = Label(rendererPool, createValidConfig());

    const auto actualName = label.getName();

    ASSERT_EQ(actualName, labelName);
}

TEST_F(LabelTest, setColor)
{
    auto label = Label(rendererPool, createValidConfig());
    EXPECT_CALL(*rendererPool, setColor(_, dummyColor));

    label.setColor(dummyColor);
}

TEST_F(LabelTest, setText)
{
    auto label = Label(rendererPool, createValidConfig());
    EXPECT_CALL(*rendererPool, setText(_, dummyText2));

    label.setText(dummyText2);

    EXPECT_CALL(*rendererPool, getText(_)).WillOnce(Return(textFromRendererPool));
    ASSERT_EQ(label.getText(), dummyText2);
}