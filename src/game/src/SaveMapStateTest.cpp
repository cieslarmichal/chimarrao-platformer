#include "SaveMapState.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"
#include "StatesMock.h"

#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class SaveMapStateTest_Base : public Test
{
public:
    SaveMapStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    StrictMock<StatesMock> states;
    std::unique_ptr<StrictMock<components::ui::UIManagerMock>> uiManagerInit{
        std::make_unique<StrictMock<components::ui::UIManagerMock>>()};
    StrictMock<components::ui::UIManagerMock>* uiManager{uiManagerInit.get()};
};

class SaveMapStateTest : public SaveMapStateTest_Base
{
public:
    TileMap tileMap{"", {0, 0}};
    SaveMapState saveMapState{window, rendererPool, states, std::move(uiManagerInit), tileMap};
};

TEST_F(SaveMapStateTest, xxx) {}