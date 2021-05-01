#include "EditorState.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"
#include "InputMock.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class EditorStateTest_Base : public Test
{
public:
    EditorStateTest_Base()
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
    std::unique_ptr<StrictMock<components::ui::UIManagerMock>> uiManagerInit{
        std::make_unique<StrictMock<components::ui::UIManagerMock>>()};
    StrictMock<components::ui::UIManagerMock>* uiManager{uiManagerInit.get()};
    TileMap tileMap{"", {0, 0}};
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
};

class EditorStateTest : public EditorStateTest_Base
{
public:
    EditorState editorState{window, rendererPool, fileAccess, states, std::move(uiManagerInit), tileMap};
};

TEST_F(EditorStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());

    editorState.activate();
}

TEST_F(EditorStateTest, deactivate_shouldDeactivateUI)
{
    EXPECT_CALL(*uiManager, deactivate());

    editorState.deactivate();
}

TEST_F(EditorStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(editorState.getType(), StateType::Editor);
}

TEST_F(EditorStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    editorState.render();
}

//TEST_F(EditorStateTest, update_shouldUpdateUI)
//{
//    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));
//
//    editorState.update(deltaTime, input);
//}