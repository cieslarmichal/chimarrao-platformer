#include "ControlsState.h"

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

class ControlsStateTest_Base : public Test
{
public:
    ControlsStateTest_Base()
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

class ControlsStateTest : public ControlsStateTest_Base
{
public:
    ControlsState controlsState{window, rendererPool, fileAccess, states, std::move(uiManagerInit)};
};

TEST_F(ControlsStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());

    controlsState.activate();
}

TEST_F(ControlsStateTest, deactivate_shouldDeactivateUI)
{
    EXPECT_CALL(*uiManager, deactivate());

    controlsState.deactivate();
}

TEST_F(ControlsStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(controlsState.getType(), StateType::Controls);
}

TEST_F(ControlsStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    controlsState.render();
}

TEST_F(ControlsStateTest, update_shouldUpdateUI)
{
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));

    controlsState.update(deltaTime, input);
}
