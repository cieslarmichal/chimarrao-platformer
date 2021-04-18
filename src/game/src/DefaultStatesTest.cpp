#include "DefaultStates.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "EditorMenuState.h"
#include "StlOperators.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class EditorMenuStateUIConfigBuilderTest : public Test
{
public:
    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::unique_ptr<components::ui::UIManagerMock> uiManagerInit{
        std::make_unique<NiceMock<components::ui::UIManagerMock>>()};
    components::ui::UIManagerMock* uiManager{uiManagerInit.get()};
    TileMap tileMap{"", {0, 0}};
    EditorMenuState editorMenuState{window, rendererPool, states, std::move(uiManagerInit), tileMap};
};

TEST_F(EditorMenuStateUIConfigBuilderTest, createEditorMenuUI)