#include "GameStateUIConfigBuilder.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "ComponentOwnersManagerMock.h"
#include "FileAccessMock.h"
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
#include "GameState.h"
#include "HealthComponent.h"
#include "ItemCollectorComponent.h"
#include "ProjectPathReader.h"

using namespace game;
using namespace components::ui;
using namespace components::core;
using namespace ::testing;

class GameStateUIConfigBuilderTest : public Test
{
public:
    GameStateUIConfigBuilderTest()
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
    }

    const unsigned int initialHealthPoints{100};
    const utils::Vector2f size{4, 4};
    const unsigned capacity1{1};
    std::shared_ptr<StrictMock<utils::TimerMock>> timer = std::make_shared<StrictMock<utils::TimerMock>>();
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<NiceMock<window::WindowMock>> window = std::make_shared<NiceMock<window::WindowMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    StrictMock<StatesMock> states;
    std::shared_ptr<components::ui::UIManagerMock> uiManager{
        std::make_shared<NiceMock<components::ui::UIManagerMock>>()};
    std::unique_ptr<StrictMock<ComponentOwnersManagerMock>> componentOwnersManagerInit{
        std::make_unique<StrictMock<ComponentOwnersManagerMock>>()};
    StrictMock<ComponentOwnersManagerMock>* componentOwnersManager{componentOwnersManagerInit.get()};
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
    std::shared_ptr<StrictMock<physics::QuadtreeMock>> quadtree =
        std::make_shared<StrictMock<physics::QuadtreeMock>>();
    std::shared_ptr<StrictMock<physics::RayCastMock>> rayCast =
        std::make_shared<StrictMock<physics::RayCastMock>>();
    std::shared_ptr<NiceMock<audio::MusicManagerMock>> musicManager =
        std::make_shared<NiceMock<audio::MusicManagerMock>>();
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

TEST_F(GameStateUIConfigBuilderTest, createGameUI)
{
    GameState gameState{window,       rendererPool,
                        fileAccess,   states,
                        uiManager,    std::move(componentOwnersManagerInit),
                        tileMap,      sharedContext,
                        musicManager, std::move(worldBuilderInit)};
    const auto gameUI = GameStateUIConfigBuilder::createGameUIConfig(&gameState);

    ASSERT_EQ(gameUI->backgroundConfig->uniqueName, "gameBackground");
    ASSERT_TRUE(gameUI->buttonsConfig.empty());
    ASSERT_TRUE(gameUI->labelsConfig.empty());
    ASSERT_TRUE(gameUI->checkBoxesConfig.empty());
    ASSERT_TRUE(gameUI->textFieldsConfig.empty());
    ASSERT_TRUE(gameUI->imagesConfig.empty());
}