#include "SaveMapState.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "TileMapMock.h"
#include "UIManagerMock.h"
#include "WindowMock.h"

#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::string mapName{"name"};
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
}

class SaveMapStateTest_Base : public Test
{
public:
    SaveMapStateTest_Base()
    {
        EXPECT_CALL(*tileMap, getName()).WillOnce(ReturnRef(mapName));
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
        EXPECT_CALL(*uiManager, setText("saveMapNameTextField", mapName));
        expectHideAllIcons();
        EXPECT_CALL(*uiManager, activateComponent("saveMapIcon1Image"));
    }

    void expectHideAllIcons()
    {
        EXPECT_CALL(*uiManager, deactivateComponent("saveMapIcon1Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("saveMapIcon2Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("saveMapIcon3Image"));
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
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
};

class SaveMapStateTest : public SaveMapStateTest_Base
{
public:
    SaveMapState saveMapState{window, rendererPool, fileAccess, states, uiManager, tileMap};
};

TEST_F(SaveMapStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("saveMapIcon1Image"));

    saveMapState.activate();
}

TEST_F(SaveMapStateTest, deactivate_shouldDeactivateUI)
{
    EXPECT_CALL(*uiManager, deactivate());

    saveMapState.deactivate();
}

TEST_F(SaveMapStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(saveMapState.getType(), StateType::SaveMap);
}

TEST_F(SaveMapStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    saveMapState.render();
}

TEST_F(SaveMapStateTest, update_shouldUpdateUI)
{
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));

    saveMapState.update(deltaTime, input);
}