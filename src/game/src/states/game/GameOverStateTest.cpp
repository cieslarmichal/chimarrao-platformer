#include "GameOverState.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "UIManagerMock.h"
#include "WindowMock.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
}

class GameOverStateTest_Base : public Test
{
public:
    GameOverStateTest_Base()
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
    StrictMock<StatesMock> states;
    std::shared_ptr<StrictMock<components::ui::UIManagerMock>> uiManager{
        std::make_shared<StrictMock<components::ui::UIManagerMock>>()};
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
};

class GameOverStateTest : public GameOverStateTest_Base
{
public:
    GameOverState gameOverState{window, rendererPool, fileAccess, states, uiManager};
};

TEST_F(GameOverStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());

    gameOverState.activate();
}

TEST_F(GameOverStateTest, deactivate_shouldDeactivateUI)
{
    EXPECT_CALL(*uiManager, deactivate());

    gameOverState.deactivate();
}

TEST_F(GameOverStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(gameOverState.getType(), StateType::GameOver);
}

TEST_F(GameOverStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    gameOverState.render();
}

TEST_F(GameOverStateTest, update_shouldUpdateUI)
{
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));

    gameOverState.update(deltaTime, input);
}