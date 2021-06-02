#include "EditorState.h"

#include <memory>

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "editor/TileMapMock.h"
#include "ui/UIManagerMock.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const utils::Vector2i positionInit{0, 0};
const utils::Vector2f position{0, 0};
const utils::Vector2f size{0, 0};
const utils::Vector2f offset{1, 1};
const utils::Vector2f newPosition{1, 1};
std::shared_ptr<Tile> tile = std::make_shared<Tile>();

const std::string mapName{"name"};
const utils::Vector2i mapSize{1, 1};
const auto grassTexturePath = utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/2.png";
}

class EditorStateTest_Base : public Test
{
public:
    EditorStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
        EXPECT_CALL(*tileMap, getSize()).WillOnce(Return(utils::Vector2i{1, 1}));
        EXPECT_CALL(*tileMap, setTileMapInfo(_));
        EXPECT_CALL(*tileMap, getTile(_)).WillRepeatedly(ReturnRef(tile));
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::unique_ptr<StrictMock<components::ui::UIManagerMock>> uiManagerInit{
        std::make_unique<StrictMock<components::ui::UIManagerMock>>()};
    StrictMock<components::ui::UIManagerMock>* uiManager{uiManagerInit.get()};
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
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

// TEST_F(EditorStateTest, update_shouldUpdateUI)
//{
//     EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));
//
//     editorState.update(deltaTime, input);
// }