#include "PaginatedButtonsNavigator.h"

#include "gtest/gtest.h"

#include "InputMock.h"
#include "TimerMock.h"
#include "UIManagerMock.h"

using namespace game;
using namespace ::testing;

namespace
{

const std::vector<std::string> validButtonNames{"button1", "button2", "button3",
                                                "button4", "button5", "button6"};

const std::vector<std::string> validIconNames{"icon1", "icon2", "icon3", "icon4", "icon5", "icon6"};
const std::vector<std::string> itemsToPaginate{"x1", "x2", "x3", "x4", "x5", "x6"};
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
}

class PaginatedButtonsNavigatorTest : public Test
{
public:
    void expectHideAllIcons()
    {
        EXPECT_CALL(*uiManager, deactivateComponent("icon1"));
        EXPECT_CALL(*uiManager, deactivateComponent("icon2"));
        EXPECT_CALL(*uiManager, deactivateComponent("icon3"));
        EXPECT_CALL(*uiManager, deactivateComponent("icon4"));
        EXPECT_CALL(*uiManager, deactivateComponent("icon5"));
        EXPECT_CALL(*uiManager, deactivateComponent("icon6"));
    }

    void expectUnselectAllButtons()
    {
        EXPECT_CALL(*uiManager, setColor("button1", buttonColor));
        EXPECT_CALL(*uiManager, setColor("button2", buttonColor));
        EXPECT_CALL(*uiManager, setColor("button3", buttonColor));
        EXPECT_CALL(*uiManager, setColor("button4", buttonColor));
        EXPECT_CALL(*uiManager, setColor("button5", buttonColor));
        EXPECT_CALL(*uiManager, setColor("button6", buttonColor));
    }

    void moveRight()
    {
        EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
        EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
        EXPECT_CALL(*moveTimer, restart());
        EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
        EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
        EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(false));
        EXPECT_CALL(input, isKeyPressed(input::InputKey::Right)).WillOnce(Return(true));
        EXPECT_CALL(*uiManager, setText("button1", "x6"));
        EXPECT_CALL(*uiManager, changeClickAction("button1", _));
        expectUnselectAllButtons();
        EXPECT_CALL(*uiManager, setColor("button1", buttonHoverColor));
        EXPECT_CALL(*uiManager, deactivateComponent("button2"));
        EXPECT_CALL(*uiManager, deactivateComponent("button3"));
        EXPECT_CALL(*uiManager, deactivateComponent("button4"));
        EXPECT_CALL(*uiManager, deactivateComponent("button5"));
        expectHideAllIcons();
        EXPECT_CALL(*uiManager, activateComponent("icon1"));

        paginatedButtonsNavigator.update(deltaTime, input);
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
    std::function<void(int)> paginatedButtonActionForButtonIndex{[&](int) {}};

    PaginatedButtonsNavigator paginatedButtonsNavigator{uiManager,
                                                        validButtonNames,
                                                        validIconNames,
                                                        itemsToPaginate,
                                                        paginatedButtonActionForButtonIndex,
                                                        5,
                                                        buttonColor,
                                                        buttonHoverColor,
                                                        std::move(moveTimerInit),
                                                        std::move(actionTimerInit)};
};

TEST_F(PaginatedButtonsNavigatorTest, givenInvalidGridButtonsInfoAndIconNames_shouldThrowRuntimeError)
{
    ASSERT_ANY_THROW(PaginatedButtonsNavigator(
        uiManager, validButtonNames, {}, itemsToPaginate, paginatedButtonActionForButtonIndex, 5, buttonColor,
        buttonHoverColor, std::move(moveTimerInit), std::move(actionTimerInit)));
}

TEST_F(PaginatedButtonsNavigatorTest,
       initialize_shouldHighlightFirstButton_andHideAllIconsAndSetAssiociatedIconVisible)
{
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("icon1"));
    EXPECT_CALL(*uiManager, setColor("button1", buttonHoverColor));

    paginatedButtonsNavigator.initialize();
}

TEST_F(PaginatedButtonsNavigatorTest, activate_shouldHideAllIconsAndSetCurrentIconVisible)
{
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("icon1"));
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
    EXPECT_CALL(*uiManager, activateComponent("icon4"));
    EXPECT_CALL(*uiManager, setColor("button4", buttonHoverColor));

    paginatedButtonsNavigator.setFocusOnButton("button4");
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
    EXPECT_CALL(*uiManager, setText("button1", "x6"));
    EXPECT_CALL(*uiManager, changeClickAction("button1", _));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("button1", buttonHoverColor));
    EXPECT_CALL(*uiManager, deactivateComponent("button2"));
    EXPECT_CALL(*uiManager, deactivateComponent("button3"));
    EXPECT_CALL(*uiManager, deactivateComponent("button4"));
    EXPECT_CALL(*uiManager, deactivateComponent("button5"));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("icon1"));

    const auto nextState = paginatedButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(PaginatedButtonsNavigatorTest,
       update_withLeftArrowClicked_shouldMoveToTheButtonOnTheLeftSide_andReturnSameState)
{
    moveRight();
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Left)).WillOnce(Return(true));
    EXPECT_CALL(*uiManager, activateComponent("button1"));
    EXPECT_CALL(*uiManager, activateComponent("button2"));
    EXPECT_CALL(*uiManager, activateComponent("button3"));
    EXPECT_CALL(*uiManager, activateComponent("button4"));
    EXPECT_CALL(*uiManager, activateComponent("button5"));
    EXPECT_CALL(*uiManager, setText("button1", "x1"));
    EXPECT_CALL(*uiManager, setText("button2", "x2"));
    EXPECT_CALL(*uiManager, setText("button3", "x3"));
    EXPECT_CALL(*uiManager, setText("button4", "x4"));
    EXPECT_CALL(*uiManager, setText("button5", "x5"));
    EXPECT_CALL(*uiManager, changeClickAction("button1", _));
    EXPECT_CALL(*uiManager, changeClickAction("button2", _));
    EXPECT_CALL(*uiManager, changeClickAction("button3", _));
    EXPECT_CALL(*uiManager, changeClickAction("button4", _));
    EXPECT_CALL(*uiManager, changeClickAction("button5", _));
    expectUnselectAllButtons();
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, setColor("button1", buttonHoverColor));
    EXPECT_CALL(*uiManager, activateComponent("icon1"));

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
    EXPECT_CALL(*uiManager, setColor("button6", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("icon6"));

    const auto nextState = paginatedButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(PaginatedButtonsNavigatorTest,
       update_withDownArrowClicked_shouldMoveToTheButtonBelow_andReturnSameState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*moveTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Up)).WillOnce(Return(false));
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Down)).WillOnce(Return(true));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("button2", buttonHoverColor));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("icon2"));

    const auto nextState = paginatedButtonsNavigator.update(deltaTime, input);

    ASSERT_EQ(nextState, NextState::Same);
}

TEST_F(PaginatedButtonsNavigatorTest, update_withEnter_shouldInvokeButtonAction_andReturnSameState)
{
    EXPECT_CALL(*moveTimer, getElapsedSeconds()).WillOnce(Return(0.f));
    EXPECT_CALL(*actionTimer, getElapsedSeconds()).WillOnce(Return(1.f));
    EXPECT_CALL(*actionTimer, restart());
    EXPECT_CALL(input, isKeyPressed(input::InputKey::Enter)).WillOnce(Return(true));
    EXPECT_CALL(*uiManager, invokeClickAction("button1", input::InputKey::MouseLeft));

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

TEST_F(PaginatedButtonsNavigatorTest, changePageToRight_withNoPagesToSwitch_shouldNotDoAnything)
{
    moveRight();

    ASSERT_NO_THROW(paginatedButtonsNavigator.changePageToRight());
}

TEST_F(PaginatedButtonsNavigatorTest, changePageToRight_shouldChangeVisibleItems)
{
    EXPECT_CALL(*uiManager, setText("button1", "x6"));
    EXPECT_CALL(*uiManager, changeClickAction("button1", _));
    expectUnselectAllButtons();
    EXPECT_CALL(*uiManager, setColor("button1", buttonHoverColor));
    EXPECT_CALL(*uiManager, deactivateComponent("button2"));
    EXPECT_CALL(*uiManager, deactivateComponent("button3"));
    EXPECT_CALL(*uiManager, deactivateComponent("button4"));
    EXPECT_CALL(*uiManager, deactivateComponent("button5"));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("icon1"));

    paginatedButtonsNavigator.changePageToRight();
}

TEST_F(PaginatedButtonsNavigatorTest, changePageToLeft_withNoPagesToSwitch_shouldNotDoAnything)
{
    ASSERT_NO_THROW(paginatedButtonsNavigator.changePageToLeft());
}

TEST_F(PaginatedButtonsNavigatorTest, changePageToLeft_shouldChangeVisibleItems)
{
    moveRight();
    EXPECT_CALL(*uiManager, activateComponent("button1"));
    EXPECT_CALL(*uiManager, activateComponent("button2"));
    EXPECT_CALL(*uiManager, activateComponent("button3"));
    EXPECT_CALL(*uiManager, activateComponent("button4"));
    EXPECT_CALL(*uiManager, activateComponent("button5"));
    EXPECT_CALL(*uiManager, setText("button1", "x1"));
    EXPECT_CALL(*uiManager, setText("button2", "x2"));
    EXPECT_CALL(*uiManager, setText("button3", "x3"));
    EXPECT_CALL(*uiManager, setText("button4", "x4"));
    EXPECT_CALL(*uiManager, setText("button5", "x5"));
    EXPECT_CALL(*uiManager, changeClickAction("button1", _));
    EXPECT_CALL(*uiManager, changeClickAction("button2", _));
    EXPECT_CALL(*uiManager, changeClickAction("button3", _));
    EXPECT_CALL(*uiManager, changeClickAction("button4", _));
    EXPECT_CALL(*uiManager, changeClickAction("button5", _));
    expectUnselectAllButtons();
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, setColor("button1", buttonHoverColor));
    EXPECT_CALL(*uiManager, activateComponent("icon1"));

    paginatedButtonsNavigator.changePageToLeft();
}
