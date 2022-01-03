
#include "gtest/gtest.h"

#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "TileMapMock.h"
#include "NewGameState.h"
#include "FileAccessMock.h"
#include "InputMock.h"

using namespace game;
using namespace ::testing;

class NewGameStateTest_Base : public Test
{
public:
    NewGameStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*tileMap, loadFromFile(_));
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    StrictMock<StatesMock> states;
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
};

class NewGameStateTest : public NewGameStateTest_Base
{
public:
    NewGameState newGameState{window, rendererPool, fileAccess, states, tileMap};
};

TEST_F(NewGameStateTest, getType_shouldReturnNewGame)
{
    ASSERT_EQ(newGameState.getType(), StateType::NewGame);
}
