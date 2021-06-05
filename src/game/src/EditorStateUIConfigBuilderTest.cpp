#include "EditorStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "TimerMock.h"
#include "WindowMock.h"
#include "editor/TileMapMock.h"
#include "ui/UIManagerMock.h"

#include "EditorState.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
std::shared_ptr<Tile> tile = std::make_shared<Tile>();
}

class EditorStateUIConfigBuilderTest_Base : public Test
{
public:
    EditorStateUIConfigBuilderTest_Base()
    {
        EXPECT_CALL(*tileMap, getSize()).WillOnce(Return(utils::Vector2i{1, 1}));
        EXPECT_CALL(*tileMap, setTileMapInfo(_));
        EXPECT_CALL(*tileMap, getTile(_)).WillRepeatedly(ReturnRef(tile));
    }
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
};

class EditorStateUIConfigBuilderTest : public EditorStateUIConfigBuilderTest_Base
{
public:
    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::shared_ptr<components::ui::UIManagerMock> uiManager{
        std::make_shared<NiceMock<components::ui::UIManagerMock>>()};
    std::unique_ptr<NiceMock<utils::TimerMock>> timerInit{std::make_unique<NiceMock<utils::TimerMock>>()};
    NiceMock<utils::TimerMock>* timer{timerInit.get()};

    EditorState editorState{window,    rendererPool, fileAccess,          states,
                            uiManager, tileMap,      std::move(timerInit)};
};

TEST_F(EditorStateUIConfigBuilderTest, createEditorUI)
{
    const auto editorUI = EditorStateUIConfigBuilder::createEditorUIConfig(&editorState);

    ASSERT_EQ(editorUI->backgroundConfig->uniqueName, "editorBackground");
    ASSERT_TRUE(editorUI->buttonsConfig.empty());
    ASSERT_TRUE(editorUI->labelsConfig.empty());
    ASSERT_TRUE(editorUI->checkBoxesConfig.empty());
    ASSERT_TRUE(editorUI->textFieldsConfig.empty());
    ASSERT_TRUE(editorUI->imagesConfig.empty());
}