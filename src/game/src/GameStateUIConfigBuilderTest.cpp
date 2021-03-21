#include "GameStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "InputManagerMock.h"
#include "RendererPoolMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "GameState.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class GameStateUIConfigBuilderTest : public Test
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
    GameState gameState{window, inputManager, rendererPool, states, std::move(uiManagerInit)};
};

TEST_F(GameStateUIConfigBuilderTest, createGameUI)
{
    const auto gameUI = GameStateUIConfigBuilder::createGameUIConfig(&gameState);

    ASSERT_EQ(gameUI->backgroundConfig->uniqueName, "gameBackground");
    ASSERT_TRUE(gameUI->buttonsConfig.empty());
    ASSERT_TRUE(gameUI->labelsConfig.empty());
    ASSERT_TRUE(gameUI->checkBoxesConfig.empty());
    ASSERT_TRUE(gameUI->textFieldsConfig.empty());
}