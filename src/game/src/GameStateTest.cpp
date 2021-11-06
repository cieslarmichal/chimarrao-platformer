#include "GameState.h"

#include "gtest/gtest.h"

#include "ComponentOwnersManagerMock.h"
#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "editor/TileMapMock.h"
#include "editor/TileMapSerializerMock.h"
#include "ui/UIManagerMock.h"

#include "ProjectPathReader.h"

using namespace game;
using namespace components::ui;
using namespace ::testing;

namespace
{
std::shared_ptr<Tile> tile = std::make_shared<Tile>();
}

class GameStateTest_Base : public Test
{
public:
    GameStateTest_Base()
    {
        EXPECT_CALL(*tileMap, getSize()).WillRepeatedly(Return(utils::Vector2i{1, 1}));
        EXPECT_CALL(*tileMap, getTile(utils::Vector2i{0, 0})).WillRepeatedly(ReturnRef(tile));
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
        EXPECT_CALL(*componentOwnersManager, add(_)).Times(8);
        EXPECT_CALL(*componentOwnersManager, processNewObjects());
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    std::shared_ptr<StrictMock<components::ui::UIManagerMock>> uiManager{
        std::make_shared<StrictMock<components::ui::UIManagerMock>>()};
    std::unique_ptr<StrictMock<ComponentOwnersManagerMock>> componentOwnersManagerInit{
        std::make_unique<StrictMock<ComponentOwnersManagerMock>>()};
    StrictMock<ComponentOwnersManagerMock>* componentOwnersManager{componentOwnersManagerInit.get()};
    StrictMock<StatesMock> states;
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
    std::shared_ptr<physics::DefaultQuadtree> quadtree = std::make_shared<physics::DefaultQuadtree>();
    std::shared_ptr<physics::DefaultRayCast> rayCast = std::make_shared<physics::DefaultRayCast>(quadtree);
};

class GameStateTest : public GameStateTest_Base
{
public:
    GameState gameState{window,  rendererPool, fileAccess,
                        states,  uiManager,    std::move(componentOwnersManagerInit),
                        tileMap, rayCast, quadtree};
};

TEST_F(GameStateTest, activate_shouldActivateUIAndOwners)
{
    EXPECT_CALL(*componentOwnersManager, activate());
    EXPECT_CALL(*uiManager, activate());

    gameState.activate();
}

TEST_F(GameStateTest, deactivate_shouldDeactivateUIAndOwners)
{
    EXPECT_CALL(*componentOwnersManager, deactivate());
    EXPECT_CALL(*uiManager, deactivate());

    gameState.deactivate();
}

TEST_F(GameStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(gameState.getType(), StateType::Game);
}

TEST_F(GameStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    gameState.render();
}

TEST_F(GameStateTest, update_shouldUpdateUIAndOwners)
{
    EXPECT_CALL(*componentOwnersManager, update(deltaTime, Ref(input)));
    EXPECT_CALL(*componentOwnersManager, processRemovals());
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));

    gameState.update(deltaTime, input);
}