#include "GameState.h"

#include "gtest/gtest.h"

#include "CollisionSystemMock.h"
#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class GameStateTest_Base : public Test
{
public:
    GameStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    std::unique_ptr<StrictMock<components::ui::UIManagerMock>> uiManagerInit{
        std::make_unique<StrictMock<components::ui::UIManagerMock>>()};
    StrictMock<components::ui::UIManagerMock>* uiManager{uiManagerInit.get()};
    std::unique_ptr<StrictMock<physics::CollisionSystemMock>> collisionSystemInit{
        std::make_unique<StrictMock<physics::CollisionSystemMock>>()};
    StrictMock<physics::CollisionSystemMock>* collisionSystem{collisionSystemInit.get()};
    StrictMock<StatesMock> states;
};

class GameStateTest : public GameStateTest_Base
{
public:
    GameState gameState{
        window, rendererPool, fileAccess, states, std::move(uiManagerInit), std::move(collisionSystemInit)};
};

TEST_F(GameStateTest, xxx) {}