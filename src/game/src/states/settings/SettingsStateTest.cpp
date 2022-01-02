#include "SettingsState.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "states/StatesMock.h"
#include "UIManagerMock.h"
#include "WindowMock.h"

#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<window::Resolution> resolutions{{640, 480},   {800, 600},   {1280, 720},
                                                  {1920, 1080}, {1920, 1440}, {3840, 2160}};
const std::vector<unsigned int> frameLimits{30, 60, 90, 120};
const window::WindowSettings windowSettings{window::DisplayMode::Window, window::Resolution{800u, 600u}, true,
                                            60u};
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const window::Resolution selectedResolution{800u, 600u};
const unsigned int selectedFrameLimit{60u};
}

class SettingsStateTest_Base : public Test
{
public:
    SettingsStateTest_Base()
    {
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
        EXPECT_CALL(*window, getSupportedResolutions()).WillOnce(Return(resolutions));
        EXPECT_CALL(*window, getSupportedFrameLimits()).WillOnce(Return(frameLimits));
        EXPECT_CALL(*window, getWindowSettings()).WillOnce(Return(windowSettings));
        EXPECT_CALL(*uiManager, setColor("settingsWindowModeButton", buttonHoverColor)).Times(2);
        EXPECT_CALL(*uiManager, setText("settingsResolutionValueLabel", toString(selectedResolution)));
        EXPECT_CALL(*uiManager, setText("settingsFrameLimitValueLabel", std::to_string(selectedFrameLimit)));
        EXPECT_CALL(*uiManager, setChecked("settingsVsyncCheckBox", true));
        expectHideAllIcons();
        EXPECT_CALL(*uiManager, activateComponent("settingsIcon1Image"));
    }

    void expectHideAllIcons()
    {
        EXPECT_CALL(*uiManager, deactivateComponent("settingsIcon1Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("settingsIcon2Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("settingsIcon3Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("settingsIcon4Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("settingsIcon5Image"));
        EXPECT_CALL(*uiManager, deactivateComponent("settingsIcon6Image"));
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

class SettingsStateTest : public SettingsStateTest_Base
{
public:
    SettingsState settingsState{window, rendererPool, fileAccess, states, uiManager};
};

TEST_F(SettingsStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());
    EXPECT_CALL(*window, getWindowSettings()).WillOnce(Return(windowSettings));
    expectHideAllIcons();
    EXPECT_CALL(*uiManager, activateComponent("settingsIcon1Image"));

    settingsState.activate();
}

TEST_F(SettingsStateTest, deactivate_shouldDeactivateUI)
{
    EXPECT_CALL(*uiManager, deactivate());

    settingsState.deactivate();
}

TEST_F(SettingsStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(settingsState.getType(), StateType::Settings);
}

TEST_F(SettingsStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    settingsState.render();
}

TEST_F(SettingsStateTest, update_shouldUpdateUI)
{
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));

    settingsState.update(deltaTime, input);
}