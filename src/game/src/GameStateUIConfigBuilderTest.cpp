#include "GameStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "ComponentOwnersManagerMock.h"
#include "FileAccessMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "editor/TileMapSerializerMock.h"
#include "ui/UIManagerMock.h"

#include "GameState.h"
#include "ProjectPathReader.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
const auto brickTexturePath = utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/brick.png";
}

class GameStateUIConfigBuilderTest : public Test
{
public:
    GameStateUIConfigBuilderTest()
    {
        EXPECT_CALL(*rendererPool, acquire(utils::Vector2f{5, 5}, utils::Vector2f{10, 10},
                                           graphics::Color{128, 91, 50}, graphics::VisibilityLayer::Second));
        EXPECT_CALL(*rendererPool, acquire(utils::Vector2f{5, 5}, utils::Vector2f{30, 30}, brickTexturePath,
                                           graphics::VisibilityLayer::Second));
        EXPECT_CALL(*componentOwnersManager, add(_)).Times(2);
        EXPECT_CALL(*componentOwnersManager, processNewObjects());
        EXPECT_CALL(*rendererPool, release(_)).Times(2);
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
    std::unique_ptr<StrictMock<ComponentOwnersManagerMock>> componentOwnersManagerInit{
        std::make_unique<StrictMock<ComponentOwnersManagerMock>>()};
    StrictMock<ComponentOwnersManagerMock>* componentOwnersManager{componentOwnersManagerInit.get()};
    std::unique_ptr<StrictMock<TileMapSerializerMock>> tileMapSerializerInit{
        std::make_unique<StrictMock<TileMapSerializerMock>>()};
    StrictMock<TileMapSerializerMock>* tileMapSerializer{tileMapSerializerInit.get()};
    std::shared_ptr<TileMap> tileMap = std::make_shared<TileMap>(
        "editorStateTestTileMap", utils::Vector2i{40, 15}, std::move(tileMapSerializerInit), fileAccess);
};

TEST_F(GameStateUIConfigBuilderTest, createGameUI)
{
    GameState gameState{window,
                        rendererPool,
                        fileAccess,
                        states,
                        std::move(uiManagerInit),
                        std::move(componentOwnersManagerInit),
                        tileMap};
    const auto gameUI = GameStateUIConfigBuilder::createGameUIConfig(&gameState);

    ASSERT_EQ(gameUI->backgroundConfig->uniqueName, "gameBackground");
    ASSERT_TRUE(gameUI->buttonsConfig.empty());
    ASSERT_TRUE(gameUI->labelsConfig.empty());
    ASSERT_TRUE(gameUI->checkBoxesConfig.empty());
    ASSERT_TRUE(gameUI->textFieldsConfig.empty());
    ASSERT_TRUE(gameUI->imagesConfig.empty());
}