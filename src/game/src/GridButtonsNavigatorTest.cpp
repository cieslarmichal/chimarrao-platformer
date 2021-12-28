#include "GridButtonsNavigator.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "TimerMock.h"
#include "UIManagerMock.h"

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

    void moveToTheFullscreenButton()
    {
        expectUnselectAllButtons();
        expectHideAllIcons();
        EXPECT_CALL(*uiManager, activateComponent("settingsIcon1Image"));
        gridButtonsNavigator.setFocusOnButton("settingsFullscreenModeButton");
    }

    void moveToTheBackButton()
    {
        expectUnselectAllButtons();
        expectHideAllIcons();
        EXPECT_CALL(*uiManager, activateComponent("settingsIcon5Image"));
        gridButtonsNavigator.setFocusOnButton("settingsBackToMenuButton");
    }

    void moveToTheApplyButton()
    {
        expectUnselectAllButtons();
        expectHideAllIcons();
        EXPECT_CALL(*uiManager, activateComponent("settingsIcon5Image"));
        gridButtonsNavigator.setFocusOnButton("settingsApplyChangesButton");
    }

    std::shared_ptr<StrictMock<components::ui::UIManagerMock>> uiManager{
        std::make_shared<StrictMock<components::ui::UIManagerMock>>()};
    std::unique_ptr<StrictMock<utils::TimerMock>> moveTimerInit{
        std::make_unique<StrictMock<utils::TimerMock>>()};
    StrictMock<utils::TimerMock>* moveTimer{moveTimerInit.get()};
    std::unique_ptr<StrictMock<utils::TimerMock>> actionTimerInit{
        std::make_unique<StrictMock<utils::TimerMock>>()};
    StrictMock<utils::TimerMock>* actionTimer{actionTimerInit.get()};
    StrictMock<input::InputMock> input;
    const utils::DeltaTime deltaTime{1};

    GridButtonsNavigator gridButtonsNavigator{uiManager,
                                              validGridButtonsInfo,
                                              validIconNames,
                                              buttonColor,
                                              buttonHoverColor,
                                              std::move(moveTimerInit),
                                              std::move(actionTimerInit)};
};

TEST_F(GridButtonsNavigatorTest, givenInvalidGridButtonsInfoAndIconNames_shouldThrowRuntimeError)
{
    ASSERT_ANY_THROW(GridButtonsNavigator(uiManager, validGridButtonsInfo, {}, buttonColor, buttonHoverColor,
                                          std::move(moveTimerInit), std::move(actionTimerInit)));
}

TEST_F(GridButtonsNavigatorTest,
       initialize_shouldHighlightFirstButton_andHideAllIconsAndSetAssiociatedIconVisible)
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
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(*actionTimer, restart());

    gridButtonsNavigator.activate();
}

TEST_F(GridButtonsNavigatorTest, setFocusOnButtonWithNonExistingButtonName_shouldNotThrow)
{
    ASSERT_NO_THROW(gridButtonsNavigator.setFocusOnButton("nonExistingButtonName"));
}

TEST_F(GridButtonsNavigatorTest,
       setFocusOnButtonWithExistingButtonName_shouldHighlightThisButtonAndCorrespondingIcon)
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

TEST_F(
    GridButtonsNavigatorTest,
    update_withRightArrowClickedAndButtonsHorizontalMoveCauseAction_shouldMoveToTheButtonOnTheRightSideAndCallItsAction)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("settingsFullscreenModeButton", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon1Image"));
    EXPECT_CALL(*uiManager, invokeClickAction("settingsFullscreenModeButton", input::InputKey::MouseLeft));
    EXPECT_CALL(*uiManager, invokeMouseOutAction("settingsWindowModeButton"));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(GridButtonsNavigatorTest,
       update_withRightArrowClickedAndButtonsHorizontalMoveNotCauseAction_shouldMoveToTheButtonOnTheRight)
{
    moveToTheBackButton();
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("settingsApplyChangesButton", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon5Image"));
    EXPECT_CALL(*uiManager, invokeMouseOutAction("settingsBackToMenuButton"));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(
    GridButtonsNavigatorTest,
    update_withLeftArrowClickedAndButtonsHorizontalMoveCauseAction_shouldMoveToTheButtonOnTheLeftSide_andReturnSameStateAndCallItsAction)
{
    moveToTheFullscreenButton();
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("settingsWindowModeButton", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon1Image"));
    EXPECT_CALL(*uiManager, invokeClickAction("settingsWindowModeButton", input::InputKey::MouseLeft));
    EXPECT_CALL(*uiManager, invokeMouseOutAction("settingsFullscreenModeButton"));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(
    GridButtonsNavigatorTest,
    update_withLeftArrowClickedAndButtonsHorizontalMoveNotCauseAction_shouldMoveToTheButtonOnTheLeftSide_andReturnSameState)
{
    moveToTheApplyButton();
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("settingsBackToMenuButton", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon5Image"));
    EXPECT_CALL(*uiManager, invokeMouseOutAction("settingsApplyChangesButton"));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(GridButtonsNavigatorTest, update_withUpArrowClicked_shouldMoveToTheButtonAbove_andReturnSameState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(true));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("settingsBackToMenuButton", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon5Image"));
    EXPECT_CALL(*uiManager, invokeMouseOutAction("settingsWindowModeButton"));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(GridButtonsNavigatorTest, update_withDownArrowClicked_shouldMoveToTheButtonBelow_andReturnSameState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(true));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("settingsResolutionDecreaseButton", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon2Image"));
    EXPECT_CALL(*uiManager, invokeMouseOutAction("settingsWindowModeButton"));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(GridButtonsNavigatorTest, update_withEnter_shouldInvokeButtonAction_andReturnSameState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Enter)).WillOnce(Return(true));
    EXPECT_CALL(*uiManager, invokeClickAction("settingsWindowModeButton", input::InputKey::MouseLeft));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(GridButtonsNavigatorTest, update_withEscape_shouldReturnPreviousState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Enter)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Escape)).WillOnce(Return(true));

    const auto nextState = gridButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Previous);
}