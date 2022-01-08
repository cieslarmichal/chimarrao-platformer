#include "GameVictoryStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "UIManagerMock.h"
#include "WindowMock.h"

#include "GameVictoryState.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> expectedLabelNames{"gameVictoryTitleLabel"};
const std::vector<std::string> expectedButtonNames{"gameVictoryBackToMenuButton"};
const std::vector<std::string> expectedImagesNames{"gameVictoryIcon1Image"};
}
class GameVictoryStateUIConfigBuilderTest : public Test
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
    GameVictoryState gameVictoryState{window, rendererPool, fileAccess, states, uiManager};
};

TEST_F(GameVictoryStateUIConfigBuilderTest, createGameVictoryStateUI)
{
    const auto gameVictoryUI = GameVictoryStateUIConfigBuilder::createGameVictoryUIConfig(&gameVictoryState);

    std::vector<std::string> actualLabelNames;
    std::transform(gameVictoryUI->labelsConfig.begin(), gameVictoryUI->labelsConfig.end(),
                   std::back_inserter(actualLabelNames),
                   [](const auto& labelConfig) { return labelConfig->uniqueName; });

    std::vector<std::string> actualButtonsNames;
    std::transform(gameVictoryUI->buttonsConfig.begin(), gameVictoryUI->buttonsConfig.end(),
                   std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    std::vector<std::string> actualImageNames;
    std::transform(gameVictoryUI->imagesConfig.begin(), gameVictoryUI->imagesConfig.end(),
                   std::back_inserter(actualImageNames),
                   [](const auto& imageConfig) { return imageConfig->uniqueName; });

    ASSERT_EQ(gameVictoryUI->backgroundConfig->uniqueName, "gameVictoryBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(gameVictoryUI->checkBoxesConfig.empty());
    ASSERT_TRUE(gameVictoryUI->textFieldsConfig.empty());
    ASSERT_TRUE(compareVectors(actualImageNames, expectedImagesNames));
}
