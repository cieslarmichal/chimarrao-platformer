#include "EditorStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "EditorState.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class EditorStateUIConfigBuilderTest : public Test
{
public:
    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::stack<std::unique_ptr<State>> states;
    std::unique_ptr<components::ui::UIManagerMock> uiManagerInit{
        std::make_unique<NiceMock<components::ui::UIManagerMock>>()};
    components::ui::UIManagerMock* uiManager{uiManagerInit.get()};
    EditorState editorState{window, rendererPool, states, std::move(uiManagerInit)};
};

TEST_F(EditorStateUIConfigBuilderTest, createEditorUI)
{
    const auto editorUI = EditorStateUIConfigBuilder::createEditorUIConfig(&editorState);

    ASSERT_EQ(editorUI->backgroundConfig->uniqueName, "editorBackground");
    ASSERT_TRUE(editorUI->buttonsConfig.empty());
    ASSERT_TRUE(editorUI->labelsConfig.empty());
    ASSERT_TRUE(editorUI->checkBoxesConfig.empty());
    ASSERT_TRUE(editorUI->textFieldsConfig.empty());
}