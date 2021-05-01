#include "SettingsState.h"

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
        EXPECT_CALL(*uiManager, setColor(components::ui::UIComponentType::Button, "settingsWindowModeButton",
                                         buttonHoverColor));
        EXPECT_CALL(*uiManager, setText(components::ui::UIComponentTypeWithLabel::Label,
                                        "settingsResolutionValueLabel", toString(selectedResolution)));
        EXPECT_CALL(*uiManager, setText(components::ui::UIComponentTypeWithLabel::Label,
                                        "settingsFrameLimitValueLabel", std::to_string(selectedFrameLimit)));
        EXPECT_CALL(*uiManager, setChecked(components::ui::UIComponentTypeWithCheck::CheckBox,
                                           "settingsVsyncCheckBox", true));
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

class SettingsStateTest : public SettingsStateTest_Base
{
public:
    SettingsState settingsState{window, rendererPool, fileAccess, states, std::move(uiManagerInit)};
};

TEST_F(SettingsStateTest, activate_shouldActivateUI)
{
    EXPECT_CALL(*uiManager, activate());
    EXPECT_CALL(*window, getWindowSettings()).WillOnce(Return(windowSettings));

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