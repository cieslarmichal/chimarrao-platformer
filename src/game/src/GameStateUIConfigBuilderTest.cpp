#include "GameStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "CollisionSystemMock.h"
#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "ui/UIManagerMock.h"

#include "GameState.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class GameStateUIConfigBuilderTest : public Test
{
public:
    GameStateUIConfigBuilderTest()
    {
        EXPECT_CALL(*collisionSystem, add(_));
    }

    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::unique_ptr<components::ui::UIManagerMock> uiManagerInit{
        std::make_unique<NiceMock<components::ui::UIManagerMock>>()};
    components::ui::UIManagerMock* uiManager{uiManagerInit.get()};
    std::unique_ptr<StrictMock<physics::CollisionSystemMock>> collisionSystemInit{
        std::make_unique<StrictMock<physics::CollisionSystemMock>>()};
    StrictMock<physics::CollisionSystemMock>* collisionSystem{collisionSystemInit.get()};
};

TEST_F(GameStateUIConfigBuilderTest, createGameUI)
{
    GameState gameState{
        window, rendererPool, fileAccess, states, std::move(uiManagerInit), std::move(collisionSystemInit)};
    const auto gameUI = GameStateUIConfigBuilder::createGameUIConfig(&gameState);

    ASSERT_EQ(gameUI->backgroundConfig->uniqueName, "gameBackground");
    ASSERT_TRUE(gameUI->buttonsConfig.empty());
    ASSERT_TRUE(gameUI->labelsConfig.empty());
    ASSERT_TRUE(gameUI->checkBoxesConfig.empty());
    ASSERT_TRUE(gameUI->textFieldsConfig.empty());
}