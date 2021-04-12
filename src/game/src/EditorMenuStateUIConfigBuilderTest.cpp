#include "EditorMenuStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "InputManagerMock.h"
#include "RendererPoolMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

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
}
class EditorMenuStateUIConfigBuilderTest : public Test
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
    TileMap tileMap{"", {0, 0}};
    EditorMenuState editorMenuState{window, inputManager, rendererPool, states, std::move(uiManagerInit),
                                    tileMap};
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

    ASSERT_EQ(editorMenuUI->backgroundConfig->uniqueName, "editorMenuBackground");
    ASSERT_TRUE(compareVectors(actualLabelNames, expectedLabelNames));
    ASSERT_TRUE(compareVectors(actualButtonsNames, expectedButtonNames));
    ASSERT_TRUE(editorMenuUI->checkBoxesConfig.empty());
    ASSERT_TRUE(editorMenuUI->textFieldsConfig.empty());
}