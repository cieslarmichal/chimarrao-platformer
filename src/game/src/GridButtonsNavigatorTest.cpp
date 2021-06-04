#include "GridButtonsNavigator.h"

#include "gtest/gtest.h"

#include "ui/UIManagerMock.h"
#include "InputMock.h"


using namespace game;
using namespace ::testing;

namespace
{
const std::vector<std::vector<GridButtonInfo>> validGridButtonsInfo{
    {GridButtonInfo{"settingsWindowModeButton", 0, true},
        GridButtonInfo{"settingsFullscreenModeButton", 0, true}},
    {GridButtonInfo{"settingsResolutionDecreaseButton", 1, true},
        GridButtonInfo{"settingsResolutionIncreaseButton", 1, true}},
    {GridButtonInfo{"settingsFrameLimitDecreaseButton", 3, true},
        GridButtonInfo{"settingsFrameLimitIncreaseButton", 3, true}},
    {GridButtonInfo{"settingsBackToMenuButton", 4, false},
        GridButtonInfo{"settingsApplyChangesButton", 4, false}}};

const std::vector<std::string> validIconNames{"settingsIcon1Image", "settingsIcon2Image",
                                        "settingsIcon3Image", "settingsIcon4Image",
                                        "settingsIcon5Image"};
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
}

class GridButtonsNavigatorTest : public Test
{
public:
    void expectHideAllIcons()
    {
        EXPECT_CALL(*uiManager, deactivateComponent("settingsIcon1Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("settingsIcon2Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("settingsIcon3Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("settingsIcon4Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("settingsIcon5Image"));
    }

    void expectUnselectAllButtons()
    {
        EXPECT_CALL(*uiManager, setColor("settingsWindowModeButton", buttonColor));
        EXPECT_CALL(*uiManager, setColor("settingsFullscreenModeButton", buttonColor));
        EXPECT_CALL(*uiManager, setColor("settingsResolutionDecreaseButton", buttonColor));
        EXPECT_CALL(*uiManager, setColor("settingsResolutionIncreaseButton", buttonColor));
        EXPECT_CALL(*uiManager, setColor("settingsFrameLimitDecreaseButton", buttonColor));
        EXPECT_CALL(*uiManager, setColor("settingsFrameLimitIncreaseButton", buttonColor));
        EXPECT_CALL(*uiManager, setColor("settingsBackToMenuButton", buttonColor));
        EXPECT_CALL(*uiManager, setColor("settingsApplyChangesButton", buttonColor));
    }

    void moveToTheButtonOnTheRightSide()
    {
        expectUnselectAllButtons();
        expectHideAllIcons();
        EXPECT_CALL(*uiManager, activateComponent("settingsIcon1Image"));
        gridButtonsNavigator.setFocusOnButton("settingsFullscreenModeButton");
    }


    std::shared_ptr<StrictMock<components::ui::UIManagerMock>> uiManager{
        std::make_shared<StrictMock<components::ui::UIManagerMock>>()};
    StrictMock<input::InputMock> input;
    const utils::DeltaTime deltaTime{1};

    GridButtonsNavigator gridButtonsNavigator{uiManager, validGridButtonsInfo, validIconNames, buttonColor, buttonHoverColor};
};

TEST_F(GridButtonsNavigatorTest, givenInvalidGridButtonsInfoAndIconNames_shouldThrowRuntimeError)
{
    ASSERT_ANY_THROW(GridButtonsNavigator(uiManager, validGridButtonsInfo, {}, buttonColor, buttonHoverColor));
}

TEST_F(GridButtonsNavigatorTest, initialize_shouldHighlightFirstButton_andHideAllIconsAndSetAssiociatedIconVisible)
{
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon1Image"));
    EXPECT_CALL(*uiManager, setColor("settingsWindowModeButton", buttonHoverColor));

    gridButtonsNavigator.initialize();
}

TEST_F(GridButtonsNavigatorTest, activate_shouldHideAllIconsAndSetCurrentIconVisible)
{
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon1Image"));

    gridButtonsNavigator.activate();
}

TEST_F(GridButtonsNavigatorTest, setFocusOnButtonWithNonExistingButtonName_shouldNotThrow)
{
    ASSERT_NO_THROW(gridButtonsNavigator.setFocusOnButton("nonExistingButtonName"));
}

TEST_F(GridButtonsNavigatorTest, setFocusOnButtonWithExistingButtonName_shouldHighlightThisButtonAndCorrespondingIcon)
{
    expectUnselectAllButtons();
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon5Image"));

    gridButtonsNavigator.setFocusOnButton("settingsBackToMenuButton");
}

TEST_F(GridButtonsNavigatorTest, loseFocus_shouldUnselectAllButtonsAndHideAllIcons)
{
    expectUnselectAllButtons();
    expectHideAllIcons();

    gridButtonsNavigator.loseFocus();
}

TEST_F(GridButtonsNavigatorTest, update_withRightArrowClicked_shouldMoveToTheButtonOnTheRightSide_andReturnSameState)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Enter)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Escape)).WillOnce(Return(false));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("settingsFullscreenModeButton", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon1Image"));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(GridButtonsNavigatorTest, update_withLeftArrowClicked_shouldMoveToTheButtonOnTheLeftSide_andReturnSameState)
{
    moveToTheButtonOnTheRightSide();
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Enter)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Escape)).WillOnce(Return(false));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("settingsWindowModeButton", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon1Image"));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(GridButtonsNavigatorTest, update_withUpArrowClicked_shouldMoveToTheButtonAbove_andReturnSameState)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Enter)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Escape)).WillOnce(Return(false));
    expectUnselectAllButtons();
    expectHideAllIcons();

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(GridButtonsNavigatorTest, update_withDownArrowClicked_shouldMoveToTheButtonBelow_andReturnSameState)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Enter)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Escape)).WillOnce(Return(false));
    expectUnselectAllButtons();
    expectHideAllIcons();

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(GridButtonsNavigatorTest, update_withEnter_shouldInvokeButtonAction_andReturnSameState)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Enter)).WillOnce(Return(true));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Escape)).WillOnce(Return(false));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(GridButtonsNavigatorTest, update_withEscape_shouldReturnPreviousState)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Enter)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Escape)).WillOnce(Return(true));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Previous);
}