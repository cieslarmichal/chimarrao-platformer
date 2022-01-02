#include "Game.h"

#include "gtest/gtest.h"

#include "InputManagerMock.h"
#include "InputMock.h"
#include "states/StatesMock.h"
#include "WindowMock.h"

using namespace game;
using namespace ::testing;

class GameTest_Base : public Test
{
public:
    GameTest_Base()
    {
        EXPECT_CALL(*states, addNextState(StateType::Menu));
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<StrictMock<input::InputManagerMock>> inputManager =
        std::make_shared<StrictMock<input::InputManagerMock>>();
    std::unique_ptr<StrictMock<StatesMock>> statesInit = std::make_unique<StrictMock<StatesMock>>();
    StrictMock<StatesMock>* states = statesInit.get();
    const StrictMock<input::InputMock> input;
};

class GameTest : public GameTest_Base
{
public:
    Game game{window, inputManager, std::move(statesInit)};
};

TEST_F(GameTest, run_withWindowClosed)
{
    EXPECT_CALL(*window, isOpen()).WillOnce(Return(true)).WillOnce(Return(false));
    EXPECT_CALL(*inputManager, readInput()).WillOnce(ReturnRef(input));
    EXPECT_CALL(*states, updateCurrentState(_, Ref(input))).WillOnce(Return(StatesStatus::Running));
    EXPECT_CALL(*states, render());
    EXPECT_CALL(*window, update());
    EXPECT_CALL(*window, display());

    game.run();
}

TEST_F(GameTest, run_withExitFromStates)
{
    EXPECT_CALL(*window, isOpen()).WillOnce(Return(true));
    EXPECT_CALL(*inputManager, readInput()).WillOnce(ReturnRef(input));
    EXPECT_CALL(*states, updateCurrentState(_, Ref(input))).WillOnce(Return(StatesStatus::Exit));
    EXPECT_CALL(*window, close());

    game.run();
}