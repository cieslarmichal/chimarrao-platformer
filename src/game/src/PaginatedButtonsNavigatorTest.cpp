#include "PaginatedButtonsNavigator.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "TimerMock.h"
#include "ui/UIManagerMock.h"

using namespace game;
using namespace ::testing;

namespace
{
const std::vector<std::string> validButtonNames{
    "menuPlayButton", "menuMapEditorButton", "menuControlsButton", "menuSettingsButton", "menuExitButton"};

const std::vector<std::string> validIconNames{"menuIcon1Image", "menuIcon2Image", "menuIcon3Image",
                                              "menuIcon4Image", "menuIcon5Image"};
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
}

class PaginatedButtonsNavigatorTest : public Test
{
public:
    void expectHideAllIcons()
    {
        EXPECT_CALL(*uiManager, deactivateComponent("menuIcon1Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("menuIcon2Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("menuIcon3Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("menuIcon4Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("menuIcon5Image"));
    }

    void expectUnselectAllButtons()
    {
        EXPECT_CALL(*uiManager, setColor("menuPlayButton", buttonColor));
        EXPECT_CALL(*uiManager, setColor("menuMapEditorButton", buttonColor));
        EXPECT_CALL(*uiManager, setColor("menuControlsButton", buttonColor));
        EXPECT_CALL(*uiManager, setColor("menuSettingsButton", buttonColor));
        EXPECT_CALL(*uiManager, setColor("menuExitButton", buttonColor));
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

    PaginatedButtonsNavigator paginatedButtonsNavigator{uiManager,   validButtonNames, validIconNames,
                                              buttonColor, buttonHoverColor,     std::move(moveTimerInit), std::move(actionTimerInit)};
};

TEST_F(PaginatedButtonsNavigatorTest, givenInvalidGridButtonsInfoAndIconNames_shouldThrowRuntimeError)
{
    ASSERT_ANY_THROW(
        PaginatedButtonsNavigator(uiManager, validButtonNames, {}, buttonColor, buttonHoverColor, std::move(moveTimerInit), std::move(actionTimerInit)));
}

TEST_F(PaginatedButtonsNavigatorTest,
       initialize_shouldHighlightFirstButton_andHideAllIconsAndSetAssiociatedIconVisible)
{
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("menuIcon1Image"));
    EXPECT_CALL(*uiManager, setColor("menuPlayButton", buttonHoverColor));

    paginatedButtonsNavigator.initialize();
}

TEST_F(PaginatedButtonsNavigatorTest, activate_shouldHideAllIconsAndSetCurrentIconVisible)
{
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("menuIcon1Image"));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(*actionTimer, restart());

    paginatedButtonsNavigator.activate();
}

TEST_F(PaginatedButtonsNavigatorTest, setFocusOnButtonWithNonExistingButtonName_shouldNotThrow)
{
    ASSERT_NO_THROW(paginatedButtonsNavigator.setFocusOnButton("nonExistingButtonName"));
}

TEST_F(PaginatedButtonsNavigatorTest,
       setFocusOnButtonWithExistingButtonName_shouldHighlightThisButtonAndCorrespondingIcon)
{
    expectUnselectAllButtons();
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("menuIcon4Image"));

    paginatedButtonsNavigator.setFocusOnButton("menuSettingsButton");
}

TEST_F(PaginatedButtonsNavigatorTest, loseFocus_shouldUnselectAllButtonsAndHideAllIcons)
{
    expectUnselectAllButtons();
    expectHideAllIcons();

    paginatedButtonsNavigator.loseFocus();
}

TEST_F(PaginatedButtonsNavigatorTest,
       update_withRightArrowClicked_shouldMoveToTheButtonOnTheRightSide_andReturnSameState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));

    const auto nextState = paginatedButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(PaginatedButtonsNavigatorTest,
       update_withLeftArrowClicked_shouldMoveToTheButtonOnTheLeftSide_andReturnSameState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));

    const auto nextState = paginatedButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(PaginatedButtonsNavigatorTest, update_withUpArrowClicked_shouldMoveToTheButtonAbove_andReturnSameState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(true));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("menuExitButton", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("menuIcon5Image"));

    const auto nextState = paginatedButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(PaginatedButtonsNavigatorTest, update_withDownArrowClicked_shouldMoveToTheButtonBelow_andReturnSameState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(true));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("menuMapEditorButton", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("menuIcon2Image"));

    const auto nextState = paginatedButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(PaginatedButtonsNavigatorTest, update_withEnter_shouldInvokeButtonAction_andReturnSameState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Enter)).WillOnce(Return(true));
    EXPECT_CALL(*uiManager, invokeClickAction("menuPlayButton", input::InputKey::MouseLeft));

    const auto nextState = paginatedButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(PaginatedButtonsNavigatorTest, update_withEscape_shouldReturnPreviousState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Enter)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Escape)).WillOnce(Return(true));

    const auto nextState = paginatedButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Previous);
}