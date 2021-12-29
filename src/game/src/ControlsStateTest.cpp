#include "ControlsState.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "UIManagerMock.h"
#include "WindowMock.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
}

class ControlsStateTest_Base : public Test
{
public:
    ControlsStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
        expectHideAllIcons();
        EXPECT_CALL(*uiManager, activateComponent("controlsIcon1Image"));
        EXPECT_CALL(*uiManager, setColor("controlsBackToMenuButton", buttonHoverColor));
    }

    void expectHideAllIcons()
    {
        EXPECT_CALL(*uiManager, deactivateComponent("controlsIcon1Image"));
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
};

class ControlsStateTest : public ControlsStateTest_Base
{
public:
    ControlsState controlsState{window, rendererPool, fileAccess, states, uiManager};
};

TEST_F(ControlsStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("controlsIcon1Image"));

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
