#include "SaveMapState.h"

#include <editor/TileMapSerializerJson.h>

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
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
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
};

class SaveMapStateTest : public SaveMapStateTest_Base
{
public:
    TileMap tileMap{"", {0, 0}, std::make_unique<TileMapSerializerJson>(),std::make_shared<utils::FileAccessMock>()};
    SaveMapState saveMapState{window, rendererPool, fileAccess, states, std::move(uiManagerInit), std::make_shared<TileMap>(std::move(tileMap))};
};

TEST_F(SaveMapStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());

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