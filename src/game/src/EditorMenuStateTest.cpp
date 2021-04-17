#include "EditorMenuState.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class EditorMenuStateTest_Base : public Test
{
public:
    EditorMenuStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    std::stack<std::unique_ptr<State>> states;
    std::unique_ptr<StrictMock<components::ui::UIManagerMock>> uiManagerInit{
        std::make_unique<StrictMock<components::ui::UIManagerMock>>()};
    StrictMock<components::ui::UIManagerMock>* uiManager{uiManagerInit.get()};
};

class EditorMenuStateTest : public EditorMenuStateTest_Base
{
public:
    TileMap tileMap{"", {0, 0}};
    EditorMenuState editorMenuState{window, rendererPool, states, std::move(uiManagerInit), tileMap};
};

TEST_F(EditorMenuStateTest, xxx) {}