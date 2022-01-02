#include "EditorState.h"

#include <memory>

#include "gtest/gtest.h"

#include "ComponentOwnersManagerMock.h"
#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "TimerMock.h"
#include "UIManagerMock.h"
#include "WindowMock.h"
#include "TileMapMock.h"

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
std::shared_ptr<TileInfo> tile = std::make_shared<TileInfo>();
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
        EXPECT_CALL(*tileMap, getSize()).WillRepeatedly(Return(utils::Vector2i{1, 1}));
        EXPECT_CALL(*tileMap, setTileMapInfo(_));
        EXPECT_CALL(*tileMap, getTile(_)).WillRepeatedly(ReturnRef(tile));
        EXPECT_CALL(*componentOwnersManager, processNewObjects());
        EXPECT_CALL(*componentOwnersManager, add(_)).Times(2);
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::shared_ptr<StrictMock<components::ui::UIManagerMock>> uiManager{
        std::make_shared<StrictMock<components::ui::UIManagerMock>>()};
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
    std::unique_ptr<StrictMock<utils::TimerMock>> timerInit{std::make_unique<StrictMock<utils::TimerMock>>()};
    StrictMock<utils::TimerMock>* timer{timerInit.get()};
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
    std::unique_ptr<StrictMock<ComponentOwnersManagerMock>> componentOwnersManagerInit{
        std::make_unique<StrictMock<ComponentOwnersManagerMock>>()};
    StrictMock<ComponentOwnersManagerMock>* componentOwnersManager{componentOwnersManagerInit.get()};
};

class EditorStateTest : public EditorStateTest_Base
{
public:
    EditorState editorState{window,    rendererPool, fileAccess,    states,
                            uiManager, tileMap,      sharedContext, std::move(componentOwnersManagerInit)};
};

TEST_F(EditorStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());
    EXPECT_CALL(*componentOwnersManager, activate());

    editorState.activate();
}

TEST_F(EditorStateTest, deactivate_shouldDeactivateUI)
{
    EXPECT_CALL(*uiManager, deactivate());
    EXPECT_CALL(*componentOwnersManager, deactivate());

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