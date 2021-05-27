#include "ChooseMapState.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "InputMock.h"
#include "MapsReaderMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "editor/TileMapSerializerMock.h"
#include "ui/UIManagerMock.h"

#include "ProjectPathReader.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> mapPaths{"x1", "x2", "x3", "x4", "x5", "x6"};
const utils::DeltaTime deltaTime{1.0};
}

class ChooseMapStateTest_Base : public Test
{
public:
    ChooseMapStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
        EXPECT_CALL(*mapsReader, readMapFilePaths()).WillOnce(Return(mapPaths));
        EXPECT_CALL(*fileAccess, getFileNameWithoutExtension(_))
            .WillOnce(Return("x1"))
            .WillOnce(Return("x2"))
            .WillOnce(Return("x3"))
            .WillOnce(Return("x4"))
            .WillOnce(Return("x5"))
            .WillOnce(Return("x6"));
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
    std::unique_ptr<StrictMock<MapsReaderMock>> mapsReaderInit{
        std::make_unique<StrictMock<MapsReaderMock>>()};
    StrictMock<MapsReaderMock>* mapsReader{mapsReaderInit.get()};
    StrictMock<input::InputMock> input;
    std::unique_ptr<StrictMock<TileMapSerializerMock>> tileMapSerializerInit{
        std::make_unique<StrictMock<TileMapSerializerMock>>()};
    StrictMock<TileMapSerializerMock>* tileMapSerializer{tileMapSerializerInit.get()};
    std::shared_ptr<TileMap> tileMap =
        std::make_shared<TileMap>("editorStateTestTileMap", utils::Vector2i{40, 15},
                                  std::move(tileMapSerializerInit), fileAccess);
};

class ChooseMapStateTest : public ChooseMapStateTest_Base
{
public:
    ChooseMapState chooseMapState{
        window, rendererPool, fileAccess, states, std::move(uiManagerInit), std::move(mapsReaderInit), tileMap};
};

TEST_F(ChooseMapStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());

    chooseMapState.activate();
}

TEST_F(ChooseMapStateTest, deactivate_shouldDeactivateUI)
{
    EXPECT_CALL(*uiManager, deactivate());

    chooseMapState.deactivate();
}

TEST_F(ChooseMapStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(chooseMapState.getType(), StateType::ChooseMap);
}

TEST_F(ChooseMapStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    chooseMapState.render();
}

TEST_F(ChooseMapStateTest, update_shouldUpdateUI)
{
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));

    chooseMapState.update(deltaTime, input);
}
