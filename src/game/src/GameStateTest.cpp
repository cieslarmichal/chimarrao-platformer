#include "GameState.h"

#include "gtest/gtest.h"

#include "CollisionSystemMock.h"
#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"
#include "InputMock.h"

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
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
};

class GameStateTest : public GameStateTest_Base
{
public:
    GameState gameState{
        window, rendererPool, fileAccess, states, std::move(uiManagerInit), std::move(collisionSystemInit)};
};

TEST_F(GameStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());

    gameState.activate();
}

TEST_F(GameStateTest, deactivate_shouldDeactivateUI)
{
    EXPECT_CALL(*uiManager, deactivate());

    gameState.deactivate();
}

TEST_F(GameStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(gameState.getType(), StateType::Game);
}

TEST_F(GameStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    gameState.render();
}

TEST_F(GameStateTest, update_shouldUpdateUI)
{
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));

    gameState.update(deltaTime, input);
}