#include "GameOverStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "UIManagerMock.h"
#include "WindowMock.h"

#include "GameOverState.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> expectedLabelNames{"gameOverTitleLabel"};
const std::vector<std::string> expectedButtonNames{"gameOverBackToMenuButton"};
const std::vector<std::string> expectedImagesNames{"gameOverIcon1Image"};
}
class GameOverStateUIConfigBuilderTest : public Test
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
    GameOverState gameOverState{window, rendererPool, fileAccess, states, uiManager};
};

TEST_F(GameOverStateUIConfigBuilderTest, createGameOverStateUI)
{
    const auto gameOverUI = GameOverStateUIConfigBuilder::createGameOverUIConfig(&gameOverState);

    std::vector<std::string> actualLabelNames;
    std::transform(gameOverUI->labelsConfig.begin(), gameOverUI->labelsConfig.end(),
                   std::back_inserter(actualLabelNames),
                   [](const auto& labelConfig) { return labelConfig->uniqueName; });

    std::vector<std::string> actualButtonsNames;
    std::transform(gameOverUI->buttonsConfig.begin(), gameOverUI->buttonsConfig.end(),
                   std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    std::vector<std::string> actualImageNames;
    std::transform(gameOverUI->imagesConfig.begin(), gameOverUI->imagesConfig.end(),
                   std::back_inserter(actualImageNames),
                   [](const auto& imageConfig) { return imageConfig->uniqueName; });

    ASSERT_EQ(gameOverUI->backgroundConfig->uniqueName, "gameOverBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(gameOverUI->checkBoxesConfig.empty());
    ASSERT_TRUE(gameOverUI->textFieldsConfig.empty());
    ASSERT_TRUE(compareVectors(actualImageNames, expectedImagesNames));
}
