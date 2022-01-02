#include "SettingsStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "UIManagerMock.h"
#include "WindowMock.h"

#include "SettingsState.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> expectedLabelNames{
    "settingsTitleLabel",           "settingsDisplayModeLabel", "settingsResolutionLabel",
    "settingsResolutionValueLabel", "settingsVsyncLabel",       "settingsFrameLimitLabel",
    "settingsFrameLimitValueLabel"};

const std::vector<std::string> expectedButtonNames{
    "settingsBackToMenuButton",         "settingsApplyChangesButton",
    "settingsWindowModeButton",         "settingsFullscreenModeButton",
    "settingsResolutionDecreaseButton", "settingsResolutionIncreaseButton",
    "settingsFrameLimitDecreaseButton", "settingsFrameLimitIncreaseButton"};
const std::vector<std::string> expectedCheckBoxesNames{"settingsVsyncCheckBox"};
const std::vector<std::string> expectedIconNames{"settingsIcon1Image", "settingsIcon2Image",
                                                 "settingsIcon3Image", "settingsIcon4Image",
                                                 "settingsIcon5Image", "settingsIcon6Image"};
}
class SettingsStateUIConfigBuilderTest : public Test
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
    SettingsState settingsState{window, rendererPool, fileAccess, states, uiManager};
};

TEST_F(SettingsStateUIConfigBuilderTest, createSettingsUI)
{
    const auto settingsUI = SettingsStateUIConfigBuilder::createSettingsUIConfig(&settingsState);

    std::vector<std::string> actualLabelNames;
    std::transform(settingsUI->labelsConfig.begin(), settingsUI->labelsConfig.end(),
                   std::back_inserter(actualLabelNames),
                   [](const auto& labelConfig) { return labelConfig->uniqueName; });

    std::vector<std::string> actualButtonsNames;
    std::transform(settingsUI->buttonsConfig.begin(), settingsUI->buttonsConfig.end(),
                   std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    std::vector<std::string> actualCheckBoxesNames;
    std::transform(settingsUI->checkBoxesConfig.begin(), settingsUI->checkBoxesConfig.end(),
                   std::back_inserter(actualCheckBoxesNames),
                   [](const auto& checkBoxConfig) { return checkBoxConfig->uniqueName; });

    std::vector<std::string> actualImagesNames;
    std::transform(settingsUI->imagesConfig.begin(), settingsUI->imagesConfig.end(),
                   std::back_inserter(actualImagesNames),
                   [](const auto& imageConfig) { return imageConfig->uniqueName; });

    ASSERT_EQ(settingsUI->backgroundConfig->uniqueName, "settingsBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(compareVectors(actualCheckBoxesNames, expectedCheckBoxesNames));
    ASSERT_TRUE(settingsUI->textFieldsConfig.empty());
    ASSERT_TRUE(compareVectors(actualImagesNames, expectedIconNames));
}

TEST_F(SettingsStateUIConfigBuilderTest, getGridButtonsInfo)
{
    const std::vector<std::vector<GridButtonInfo>> expectedGridButtonsInfo{
        {GridButtonInfo{"settingsWindowModeButton", 0, true, true},
         GridButtonInfo{"settingsFullscreenModeButton", 0, true, true}},
        {GridButtonInfo{"settingsResolutionDecreaseButton", 1, true, false},
         GridButtonInfo{"settingsResolutionIncreaseButton", 1, true, false}},
        {GridButtonInfo{"settingsVsyncCheckBox", 2, false, false}},
        {GridButtonInfo{"settingsFrameLimitDecreaseButton", 3, true, false},
         GridButtonInfo{"settingsFrameLimitIncreaseButton", 3, true, false}},
        {GridButtonInfo{"settingsBackToMenuButton", 4, false, false},
         GridButtonInfo{"settingsApplyChangesButton", 5, false, false}}};

    const auto actualGridButtonsInfo = SettingsStateUIConfigBuilder::getGridButtonsInfo();

    ASSERT_EQ(actualGridButtonsInfo, expectedGridButtonsInfo);
}

TEST_F(SettingsStateUIConfigBuilderTest, getIconNames)
{
    const auto actualIconNames = SettingsStateUIConfigBuilder::getIconNames();

    ASSERT_EQ(actualIconNames, expectedIconNames);
}
