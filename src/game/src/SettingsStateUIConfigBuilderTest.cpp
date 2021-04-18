#include "SettingsStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

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
    std::unique_ptr<components::ui::UIManagerMock> uiManagerInit{
        std::make_unique<NiceMock<components::ui::UIManagerMock>>()};
    components::ui::UIManagerMock* uiManager{uiManagerInit.get()};
    SettingsState settingsState{window, rendererPool, fileAccess, states, std::move(uiManagerInit)};
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

    ASSERT_EQ(settingsUI->backgroundConfig->uniqueName, "settingsBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(compareVectors(actualCheckBoxesNames, expectedCheckBoxesNames));
    ASSERT_TRUE(settingsUI->textFieldsConfig.empty());
}