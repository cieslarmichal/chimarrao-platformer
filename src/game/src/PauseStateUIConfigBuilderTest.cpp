#include "PauseStateUIConfigBuilder.h"

#include <FileAccessMock.h>

#include "gtest/gtest.h"

#include "InputManagerMock.h"
#include "RendererPoolMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "PauseState.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> expectedLabelNames{"pauseTitleLabel"};

const std::vector<std::string> expectedButtonNames{"pauseBackToGameButton", "pauseBackToMenuButton"};
}
class PauseStateUIConfigBuilderTest : public Test
{
public:
    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<input::InputManagerMock>> inputManager =
        std::make_shared<NiceMock<input::InputManagerMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<NiceMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<NiceMock<utils::FileAccessMock>>();
    std::stack<std::unique_ptr<State>> states;
    std::unique_ptr<components::ui::UIManagerMock> uiManagerInit{
        std::make_unique<NiceMock<components::ui::UIManagerMock>>()};
    components::ui::UIManagerMock* uiManager{uiManagerInit.get()};
    PauseState pauseState{window, inputManager, rendererPool, fileAccess, states, std::move(uiManagerInit)};
};

TEST_F(PauseStateUIConfigBuilderTest, createPauseUI)
{
    const auto pauseUI = PauseStateUIConfigBuilder::createPauseUIConfig(&pauseState);

    std::vector<std::string> actualLabelNames;
    std::transform(pauseUI->labelsConfig.begin(), pauseUI->labelsConfig.end(),
                   std::back_inserter(actualLabelNames),
                   [](const auto& labelConfig) { return labelConfig->uniqueName; });

    std::vector<std::string> actualButtonsNames;
    std::transform(pauseUI->buttonsConfig.begin(), pauseUI->buttonsConfig.end(),
                   std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    ASSERT_EQ(pauseUI->backgroundConfig->uniqueName, "pauseBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(pauseUI->checkBoxesConfig.empty());
    ASSERT_TRUE(pauseUI->textFieldsConfig.empty());
}