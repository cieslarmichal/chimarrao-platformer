#include "LimitedSpaceActionComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "InputMock.h"
#include "ItemEffectMock.h"
#include "RendererPoolMock.h"
#include "TimerMock.h"

#include "ComponentOwner.h"
#include "DefaultQuadtree.h"
#include "DefaultRayCast.h"
#include "ProjectPathReader.h"
#include "exceptions/DependentComponentNotFound.h"

using namespace components::core;
using namespace ::testing;

namespace
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto dummyFontSize = 4;
const auto fontPath = projectPath + "resources/fonts/VeraMono.ttf";
}

class LimitedSpaceActionComponentTest : public Test
{
public:
    LimitedSpaceActionComponentTest()
    {
        owner.addComponent<TextComponent>(rendererPool, position, "press E to action", fontPath, dummyFontSize);
        owner.loadDependentComponents();

        player.addComponent<VelocityComponent>();
        player.addComponent<DirectionComponent>();
        player.addComponent<AnimationComponent>(animator);
        player.addComponent<BoxColliderComponent>(size);
        player.addComponent<ItemCollectorComponent>(quadtree, rayCast, capacity, timer);
        player.loadDependentComponents();

        limitedSpaceActionComponent.loadDependentComponents();
    }

    void clickAction(int& actionVariableInit)
    {
        actionVariableInit = clickValue;
    }

    bool actionPerformed(int actionVariableInit) const
    {
        return actionVariableInit == clickValue;
    }

    int actionVariable{0};
    const int clickValue{42};
    const unsigned capacity{1};
    const utils::Vector2f size{4, 4};
    const utils::Vector2f position{0, 0};
    const utils::Vector2f position1{8, 0};
    const utils::Vector2f position2{2, 0};
    const utils::Vector2f position3{3, 0};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner owner{position1, "LimitedSpaceActionComponentTest1", sharedContext};
    ComponentOwner player{position2, "LimitedSpaceActionComponentTest2", sharedContext};
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<StrictMock<ItemEffectMock>> itemEffect = std::make_shared<StrictMock<ItemEffectMock>>();
    std::shared_ptr<physics::Quadtree> quadtree = std::make_shared<physics::DefaultQuadtree>();
    std::shared_ptr<physics::RayCast> rayCast = std::make_shared<physics::DefaultRayCast>(quadtree);
    std::shared_ptr<StrictMock<utils::TimerMock>> timer{std::make_shared<StrictMock<utils::TimerMock>>()};
    StrictMock<input::InputMock> input;
    const utils::DeltaTime deltaTime{1};
    LimitedSpaceActionComponent limitedSpaceActionComponent{&owner, &player,
                                                            [this] { clickAction(actionVariable); }};
};

TEST_F(LimitedSpaceActionComponentTest,
       loadDependentComponentsWithoutTextComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutText{position, "componentOwnerWithoutTextComponent", sharedContext};
    LimitedSpaceActionComponent limitedSpaceActionComponentWithoutTextComponent{
        &componentOwnerWithoutText, &player, [this] { clickAction(actionVariable); }};

    ASSERT_THROW(limitedSpaceActionComponentWithoutTextComponent.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(LimitedSpaceActionComponentTest,
       loadDependentComponentsWithoutPlayersItemCollector_shouldThrowDependentComponentNotFound)
{
    ComponentOwner playerWithoutItemCollector{position, "playerWithoutItemCollectorComponent", sharedContext};
    LimitedSpaceActionComponent limitedSpaceActionComponentWithoutItemCollector{
        &owner, &playerWithoutItemCollector, [this] { clickAction(actionVariable); }};

    ASSERT_THROW(limitedSpaceActionComponentWithoutItemCollector.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(LimitedSpaceActionComponentTest, distanceBetweenOwnerAndPlayerIsMoreThan15_shouldNotCallAction)
{
    player.transform->setPosition(100, 50);

    limitedSpaceActionComponent.update(deltaTime, input);

    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(
    LimitedSpaceActionComponentTest,
    distanceBetweenOwnerAndPlayerIsLessThan10AndKeyNotPressed_shouldDisableItemCollectorAndEnableTextAndNotCallAction)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::E)).WillOnce(Return(false));

    limitedSpaceActionComponent.update(deltaTime, input);

    ASSERT_TRUE(owner.getComponent<TextComponent>()->isEnabled());
    ASSERT_FALSE(player.getComponent<ItemCollectorComponent>()->isEnabled());
    ASSERT_FALSE(actionPerformed(actionVariable));
}

TEST_F(
    LimitedSpaceActionComponentTest,
    distanceBetweenOwnerAndPlayerIsLessThan10AndKeyPressed_shouldEnableItemCollectorAndDisableTextAndCallAction)
{
    EXPECT_CALL(input, isKeyPressed(input::InputKey::E)).WillOnce(Return(true));

    limitedSpaceActionComponent.update(deltaTime, input);

    ASSERT_FALSE(owner.getComponent<TextComponent>()->isEnabled());
    ASSERT_TRUE(player.getComponent<ItemCollectorComponent>()->isEnabled());
    ASSERT_TRUE(actionPerformed(actionVariable));
}
