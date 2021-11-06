#include "GameStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "ComponentOwnersManagerMock.h"
#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "editor/TileMapMock.h"
#include "editor/TileMapSerializerMock.h"
#include "ui/UIManagerMock.h"

#include "GameState.h"
#include "ProjectPathReader.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

class GameStateUIConfigBuilderTest : public Test
{
public:
    GameStateUIConfigBuilderTest()
    {
        EXPECT_CALL(*componentOwnersManager, add(_)).Times(8);
        EXPECT_CALL(*componentOwnersManager, processNewObjects());
        EXPECT_CALL(*tileMap, getSize()).WillRepeatedly(Return(utils::Vector2i{1, 1}));
        EXPECT_CALL(*tileMap, getTile(utils::Vector2i{0, 0})).WillRepeatedly(ReturnRef(tile));
    }

    std::shared_ptr<Tile> tile = std::make_shared<Tile>();
    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::shared_ptr<components::ui::UIManagerMock> uiManager{
        std::make_shared<NiceMock<components::ui::UIManagerMock>>()};
    std::unique_ptr<StrictMock<ComponentOwnersManagerMock>> componentOwnersManagerInit{
        std::make_unique<StrictMock<ComponentOwnersManagerMock>>()};
    StrictMock<ComponentOwnersManagerMock>* componentOwnersManager{componentOwnersManagerInit.get()};
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
    std::shared_ptr<physics::DefaultQuadtree> quadtree = std::make_shared<physics::DefaultQuadtree>();
    std::shared_ptr<physics::RayCast> rayCast = std::make_shared<physics::RayCast>(quadtree);
};

TEST_F(GameStateUIConfigBuilderTest, createGameUI)
{
    GameState gameState{window,  rendererPool, fileAccess,
                        states,  uiManager,    std::move(componentOwnersManagerInit),
                        tileMap, rayCast, quadtree};
    const auto gameUI = GameStateUIConfigBuilder::createGameUIConfig(&gameState);

    ASSERT_EQ(gameUI->backgroundConfig->uniqueName, "gameBackground");
    ASSERT_TRUE(gameUI->buttonsConfig.empty());
    ASSERT_TRUE(gameUI->labelsConfig.empty());
    ASSERT_TRUE(gameUI->checkBoxesConfig.empty());
    ASSERT_TRUE(gameUI->textFieldsConfig.empty());
    ASSERT_TRUE(gameUI->imagesConfig.empty());
}