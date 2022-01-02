#include "ControlsStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "UIManagerMock.h"
#include "WindowMock.h"

#include "ControlsState.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> expectedLabelNames{"controlsTitleLabel",
                                                  "controlsUpDescriptionLabel",
                                                  "controlsDownDescriptionLabel",
                                                  "controlsRightDescriptionLabel",
                                                  "controlsLeftDescriptionLabel",
                                                  "controlsSpaceDescriptionLabel",
                                                  "controlsShiftDescriptionLabel",
                                                  "controlsCtrlDescriptionLabel",
                                                  "controlsEDescriptionLabel"};

const std::vector<std::string> expectedButtonNames{
    "controlsBackToMenuButton", "controlsUpButton",   "controlsDownButton",
    "controlsRightButton",      "controlsLeftButton", "controlsSpaceButton",
    "controlsShiftButton",      "controlsCtrlButton", "controlsEButton"};
const std::vector<std::string> expectedImagesNames{"controlsIcon1Image"};
}

class ControlsStateUIConfigBuilderTest : public Test
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
    ControlsState controlsState{window, rendererPool, fileAccess, states, uiManager};
};

TEST_F(ControlsStateUIConfigBuilderTest, createControlsUI)
{
    const auto controlsUI = ControlsStateUIConfigBuilder::createControlsUIConfig(&controlsState);

    std::vector<std::string> actualLabelNames;
    std::transform(controlsUI->labelsConfig.begin(), controlsUI->labelsConfig.end(),
                   std::back_inserter(actualLabelNames),
                   [](const auto& labelConfig) { return labelConfig->uniqueName; });

    std::vector<std::string> actualButtonsNames;
    std::transform(controlsUI->buttonsConfig.begin(), controlsUI->buttonsConfig.end(),
                   std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    std::vector<std::string> actualImageNames;
    std::transform(controlsUI->imagesConfig.begin(), controlsUI->imagesConfig.end(),
                   std::back_inserter(actualImageNames),
                   [](const auto& imageConfig) { return imageConfig->uniqueName; });

    ASSERT_EQ(controlsUI->backgroundConfig->uniqueName, "controlsBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(controlsUI->checkBoxesConfig.empty());
    ASSERT_TRUE(controlsUI->textFieldsConfig.empty());
    ASSERT_TRUE(compareVectors(actualImageNames, expectedImagesNames));
}

TEST_F(ControlsStateUIConfigBuilderTest, getGridButtonNames)
{
    std::vector<std::vector<GridButtonInfo>> expectedGridButtonsInfo{
        {GridButtonInfo{"controlsBackToMenuButton", 0, false, false}}};

    const auto actualGridButtonsInfo = ControlsStateUIConfigBuilder::getGridButtonsInfo();

    ASSERT_EQ(actualGridButtonsInfo, expectedGridButtonsInfo);
}

TEST_F(ControlsStateUIConfigBuilderTest, getIconNames)
{
    const auto actualIconNames = ControlsStateUIConfigBuilder::getIconNames();

    ASSERT_EQ(actualIconNames, expectedImagesNames);
}