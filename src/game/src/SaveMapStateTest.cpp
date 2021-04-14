#include "SaveMapState.h"

#include <FileAccessMock.h>

#include "gtest/gtest.h"

#include "InputManagerMock.h"
#include "RendererPoolMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class SaveMapStateTest_Base : public Test
{
public:
    SaveMapStateTest_Base()
    {
        EXPECT_CALL(*inputManager, registerObserver(_));
        EXPECT_CALL(*inputManager, removeObserver(_));
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
    }

    std::shared_ptr<StrictMock<input::InputManagerMock>> inputManager =
        std::make_shared<StrictMock<input::InputManagerMock>>();
    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    std::stack<std::unique_ptr<State>> states;
    std::unique_ptr<StrictMock<components::ui::UIManagerMock>> uiManagerInit{
        std::make_unique<StrictMock<components::ui::UIManagerMock>>()};
    StrictMock<components::ui::UIManagerMock>* uiManager{uiManagerInit.get()};
};

class SaveMapStateTest : public SaveMapStateTest_Base
{
public:
    TileMap tileMap{"", {0, 0}};
    SaveMapState saveMapState{window, inputManager, rendererPool, fileAccess, states, std::move(uiManagerInit), tileMap};
};

TEST_F(SaveMapStateTest, xxx) {}