#include "EditorMenuStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "TileMapMock.h"
#include "UIManagerMock.h"
#include "WindowMock.h"

#include "EditorMenuState.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const std::vector<std::string> expectedLabelNames{"editorMenuTitleLabel"};
const std::vector<std::string> expectedButtonNames{"editorMenuBackToEditorButton", "editorMenuLoadMapButton",
                                                   "editorMenuNewMapButton", "editorMenuSaveMapButton",
                                                   "editorMenuBackToMenuButton"};
const std::vector<std::string> expectedImagesNames{"editorMenuIcon1Image", "editorMenuIcon2Image",
                                                   "editorMenuIcon3Image", "editorMenuIcon4Image",
                                                   "editorMenuIcon5Image"};
}
class EditorMenuStateUIConfigBuilderTest : public Test
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
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();

    EditorMenuState editorMenuState{window, rendererPool, fileAccess, states, uiManager, tileMap};
};

TEST_F(EditorMenuStateUIConfigBuilderTest, createEditorMenuUI)
{
    const auto editorMenuUI = EditorMenuStateUIConfigBuilder::createEditorMenuUIConfig(&editorMenuState);

    std::vector<std::string> actualLabelNames;
    std::transform(editorMenuUI->labelsConfig.begin(), editorMenuUI->labelsConfig.end(),
                   std::back_inserter(actualLabelNames),
                   [](const auto& labelConfig) { return labelConfig->uniqueName; });

    std::vector<std::string> actualButtonsNames;
    std::transform(editorMenuUI->buttonsConfig.begin(), editorMenuUI->buttonsConfig.end(),
                   std::back_inserter(actualButtonsNames),
                   [](const auto& buttonConfig) { return buttonConfig->uniqueName; });

    std::vector<std::string> actualImageNames;
    std::transform(editorMenuUI->imagesConfig.begin(), editorMenuUI->imagesConfig.end(),
                   std::back_inserter(actualImageNames),
                   [](const auto& imageConfig) { return imageConfig->uniqueName; });

    ASSERT_EQ(editorMenuUI->backgroundConfig->uniqueName, "editorMenuBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(editorMenuUI->checkBoxesConfig.empty());
    ASSERT_TRUE(editorMenuUI->textFieldsConfig.empty());
    ASSERT_TRUE(compareVectors(actualImageNames, expectedImagesNames));
}

TEST_F(EditorMenuStateUIConfigBuilderTest, getGridButtonNames)
{
    const std::vector<std::vector<GridButtonInfo>> expectedGridButtonsInfo{
        {GridButtonInfo{"editorMenuBackToEditorButton", 0, false, false}},
        {GridButtonInfo{"editorMenuLoadMapButton", 1, false, false}},
        {GridButtonInfo{"editorMenuNewMapButton", 2, false, false}},
        {GridButtonInfo{"editorMenuSaveMapButton", 3, false, false}},
        {GridButtonInfo{"editorMenuBackToMenuButton", 4, false, false}}};

    const auto actualGridButtonsInfo = EditorMenuStateUIConfigBuilder::getGridButtonsInfo();

    ASSERT_EQ(actualGridButtonsInfo, expectedGridButtonsInfo);
}

TEST_F(EditorMenuStateUIConfigBuilderTest, getIconNames)
{
    const auto actualIconNames = EditorMenuStateUIConfigBuilder::getIconNames();

    ASSERT_EQ(actualIconNames, expectedImagesNames);
}