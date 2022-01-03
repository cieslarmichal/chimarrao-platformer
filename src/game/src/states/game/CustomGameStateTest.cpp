#include "CustomGameState.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "ComponentOwnersManagerMock.h"
#include "FileAccessMock.h"
#include "InputMock.h"
#include "MusicManagerMock.h"
#include "QuadtreeMock.h"
#include "RayCastMock.h"
#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "TimerMock.h"
#include "UIManagerMock.h"
#include "WindowMock.h"
#include "WorldBuilderMock.h"
#include "TileMapMock.h"

#include "AnimationComponent.h"
#include "DirectionComponent.h"
#include "HealthComponent.h"
#include "ItemCollectorComponent.h"
#include "ProjectPathReader.h"

using namespace game;
using namespace components::ui;
using namespace components::core;
using namespace ::testing;

class CustomGameStateTest_Base : public Test
{
public:
    CustomGameStateTest_Base()
    {
        owner1->addComponent<HealthComponent>(initialHealthPoints);
        owner1->addComponent<DirectionComponent>();
        owner1->addComponent<VelocityComponent>();
        owner1->addComponent<AnimationComponent>(animator);
        owner1->addComponent<BoxColliderComponent>(size);
        owner1->addComponent<ItemCollectorComponent>(quadtree, rayCast, capacity1, timer);
        owner1->loadDependentComponents();
        EXPECT_CALL(*worldBuilder, buildWorldObjects(_)).WillOnce(Return(worldObjects));
        EXPECT_CALL(*worldBuilder, getPlayer()).WillOnce(Return(owner1));
        EXPECT_CALL(*componentOwnersManager, processNewObjects());
        EXPECT_CALL(*componentOwnersManager, add(_)).Times(2);
        EXPECT_CALL(*window, registerObserver(_));
        EXPECT_CALL(*window, removeObserver(_));
        EXPECT_CALL(*uiManager, createUI(_));
        EXPECT_CALL(*musicManager, acquire(_));
        EXPECT_CALL(*musicManager, play(_));
        EXPECT_CALL(*musicManager, setVolume(_, _));
    }

    const unsigned int initialHealthPoints{100};
    const utils::Vector2f size{4, 4};
    const unsigned capacity1{1};
    std::shared_ptr<StrictMock<utils::TimerMock>> timer = std::make_shared<StrictMock<utils::TimerMock>>();
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
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
    std::shared_ptr<StrictMock<physics::QuadtreeMock>> quadtree =
        std::make_shared<StrictMock<physics::QuadtreeMock>>();
    std::shared_ptr<StrictMock<physics::RayCastMock>> rayCast =
        std::make_shared<StrictMock<physics::RayCastMock>>();
    std::shared_ptr<StrictMock<audio::MusicManagerMock>> musicManager =
        std::make_shared<StrictMock<audio::MusicManagerMock>>();
    std::unique_ptr<StrictMock<WorldBuilderMock>> worldBuilderInit{
        std::make_unique<StrictMock<WorldBuilderMock>>()};
    StrictMock<WorldBuilderMock>* worldBuilder{worldBuilderInit.get()};
    const utils::Vector2f position1{0, 10};
    std::shared_ptr<components::core::ComponentOwner> owner1 =
        std::make_shared<components::core::ComponentOwner>(position1, "GameStateUIConfigBuilderTest1",
                                                           sharedContext);
    std::shared_ptr<components::core::ComponentOwner> owner2 =
        std::make_shared<components::core::ComponentOwner>(position1, "GameStateUIConfigBuilderTest2",
                                                           sharedContext);
    std::vector<std::shared_ptr<components::core::ComponentOwner>> worldObjects{owner1, owner2};
};

class CustomGameStateTest : public CustomGameStateTest_Base
{
public:
    CustomGameState gameState{window,       rendererPool,
                        fileAccess,   states,
                        uiManager,    std::move(componentOwnersManagerInit),
                        tileMap,      sharedContext,
                        musicManager, std::move(worldBuilderInit)};
};

TEST_F(CustomGameStateTest, activate_shouldActivateUIAndOwners)
{
    EXPECT_CALL(*componentOwnersManager, activate());
    EXPECT_CALL(*uiManager, activate());
    EXPECT_CALL(*musicManager, play(_));

    gameState.activate();
}

TEST_F(CustomGameStateTest, deactivate_shouldDeactivateUIAndOwners)
{
    EXPECT_CALL(*componentOwnersManager, deactivate());
    EXPECT_CALL(*uiManager, deactivate());
    EXPECT_CALL(*musicManager, stop(_));

    gameState.deactivate();
}

TEST_F(CustomGameStateTest, getType_shouldReturnChooseMap)
{
    ASSERT_EQ(gameState.getType(), StateType::CustomGame);
}

TEST_F(CustomGameStateTest, render_shouldRenderAllFromRendererPool)
{
    EXPECT_CALL(*rendererPool, renderAll());

    gameState.render();
}

TEST_F(CustomGameStateTest, update_shouldUpdateUIAndOwners)
{
    EXPECT_CALL(*componentOwnersManager, update(deltaTime, Ref(input)));
    EXPECT_CALL(*componentOwnersManager, processRemovals());
    EXPECT_CALL(*uiManager, update(deltaTime, Ref(input)));

    gameState.update(deltaTime, input);
}