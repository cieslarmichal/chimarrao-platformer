#include "ControlsStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "InputManagerMock.h"
#include "RendererPoolMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"
#include "StlOperators.h"

#include "ControlsState.h"

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
    "controlsBackToMenuButton",
    "controlsUpButton",
    "controlsDownButton",
    "controlsRightButton",
    "controlsLeftButton",
    "controlsSpaceButton",
    "controlsShiftButton",
    "controlsCtrlButton",
    "controlsEButton"
};
}

class ControlsStateUIConfigBuilderTest : public Test
{
public:
    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<input::InputManagerMock>> inputManager =
        std::make_shared<NiceMock<input::InputManagerMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::stack<std::unique_ptr<State>> states;
    std::unique_ptr<components::ui::UIManagerMock> uiManagerInit{
        std::make_unique<NiceMock<components::ui::UIManagerMock>>()};
    components::ui::UIManagerMock* uiManager{uiManagerInit.get()};
    ControlsState controlsState{window, inputManager, rendererPool, states, std::move(uiManagerInit)};
};

TEST_F(ControlsStateUIConfigBuilderTest, createControlsUI)
{
    const auto controlsUI = ControlsStateUIConfigBuilder::createControlsUIConfig(&controlsState);

    std::vector<std::string> actualLabelNames;
    std::transform(controlsUI->labelsConfig.begin(), controlsUI->labelsConfig.end(), std::back_inserter(actualLabelNames),
                   [](const auto& labelConfig) { return labelConfig->uniqueName; });

    std::vector<std::string> actualButtonsNames;
    std::transform(controlsUI->buttonsConfig.begin(), controlsUI->buttonsConfig.end(), std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    ASSERT_EQ(controlsUI->backgroundConfig->uniqueName, "controlsBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(controlsUI->checkBoxesConfig.empty());
    ASSERT_TRUE(controlsUI->textFieldsConfig.empty());
}