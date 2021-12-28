#include "DefaultCollisionSystem.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "RendererPoolMock.h"

#include "DefaultQuadtree.h"
#include "DefaultRayCast.h"
#include "StlOperators.h"
#include "AnimationComponent.h"
#include "DirectionComponent.h"
#include "KeyboardAnimatedMovementComponent.h"

using namespace physics;
using namespace components::core;
using namespace ::testing;

class DefaultCollisionSystemTest : public Test
{
public:
    DefaultCollisionSystemTest()
    {
        componentOwnerWithDefaultCollider1->addComponent<BoxColliderComponent>(size, CollisionLayer::Default);
        componentOwnerWithDefaultCollider2->addComponent<BoxColliderComponent>(size, CollisionLayer::Default);
        componentOwnerWithTileCollider1->addComponent<BoxColliderComponent>(size, CollisionLayer::Tile);
        componentOwnerWithTileCollider2->addComponent<BoxColliderComponent>(size, CollisionLayer::Tile);
        componentOwnerWithPlayerCollider1->addComponent<BoxColliderComponent>(size, CollisionLayer::Player);
        componentOwnerWithPlayerCollider2->addComponent<BoxColliderComponent>(size, CollisionLayer::Player);
        componentOwnerWitStaticTransform->addComponent<BoxColliderComponent>(size, CollisionLayer::Player);

        componentOwnerWithDefaultCollider1->addComponent<AnimationComponent>(animator);
        componentOwnerWithDefaultCollider2->addComponent<AnimationComponent>(animator);
        componentOwnerWithTileCollider1->addComponent<AnimationComponent>(animator);
        componentOwnerWithTileCollider2->addComponent<AnimationComponent>(animator);
        componentOwnerWithPlayerCollider1->addComponent<AnimationComponent>(animator);
        componentOwnerWithPlayerCollider2->addComponent<AnimationComponent>(animator);
        componentOwnerWitStaticTransform->addComponent<AnimationComponent>(animator);

        componentOwnerWithDefaultCollider1->addComponent<VelocityComponent>();
        componentOwnerWithDefaultCollider2->addComponent<VelocityComponent>();
        componentOwnerWithTileCollider1->addComponent<VelocityComponent>();
        componentOwnerWithTileCollider2->addComponent<VelocityComponent>();
        componentOwnerWithPlayerCollider1->addComponent<VelocityComponent>();
        componentOwnerWithPlayerCollider2->addComponent<VelocityComponent>();
        componentOwnerWitStaticTransform->addComponent<VelocityComponent>();

        componentOwnerWithDefaultCollider1->addComponent<DirectionComponent>();
        componentOwnerWithDefaultCollider2->addComponent<DirectionComponent>();
        componentOwnerWithTileCollider1->addComponent<DirectionComponent>();
        componentOwnerWithTileCollider2->addComponent<DirectionComponent>();
        componentOwnerWithPlayerCollider1->addComponent<DirectionComponent>();
        componentOwnerWithPlayerCollider2->addComponent<DirectionComponent>();
        componentOwnerWitStaticTransform->addComponent<DirectionComponent>();

        componentOwnerWithDefaultCollider1->addComponent<KeyboardAnimatedMovementComponent>();
        componentOwnerWithDefaultCollider2->addComponent<KeyboardAnimatedMovementComponent>();
        componentOwnerWithTileCollider1->addComponent<KeyboardAnimatedMovementComponent>();
        componentOwnerWithTileCollider2->addComponent<KeyboardAnimatedMovementComponent>();
        componentOwnerWithPlayerCollider1->addComponent<KeyboardAnimatedMovementComponent>();
        componentOwnerWithPlayerCollider2->addComponent<KeyboardAnimatedMovementComponent>();
        componentOwnerWitStaticTransform->addComponent<KeyboardAnimatedMovementComponent>();

        componentOwnerWithDefaultCollider1->loadDependentComponents();
        componentOwnerWithDefaultCollider2->loadDependentComponents();
        componentOwnerWithTileCollider1->loadDependentComponents();
        componentOwnerWithTileCollider2->loadDependentComponents();
        componentOwnerWithPlayerCollider1->loadDependentComponents();
        componentOwnerWithPlayerCollider2->loadDependentComponents();
        componentOwnerWitStaticTransform->loadDependentComponents();
    }

    static bool canMoveLeft(const std::shared_ptr<ComponentOwner>& componentOwner)
    {
        auto movementComponent = componentOwner->getComponent<KeyboardAnimatedMovementComponent>();
        return movementComponent->isAllowedToMoveLeft();
    }

    static bool canMoveRight(const std::shared_ptr<ComponentOwner>& componentOwner)
    {
        auto movementComponent = componentOwner->getComponent<KeyboardAnimatedMovementComponent>();
        return movementComponent->isAllowedToMoveRight();
    }

    static bool canMoveUp(const std::shared_ptr<ComponentOwner>& componentOwner)
    {
        auto movementComponent = componentOwner->getComponent<KeyboardAnimatedMovementComponent>();
        return movementComponent->isAllowedToMoveUp();
    }

    static bool canMoveDown(const std::shared_ptr<ComponentOwner>& componentOwner)
    {
        auto movementComponent = componentOwner->getComponent<KeyboardAnimatedMovementComponent>();
        return movementComponent->isAllowedToMoveDown();
    }

    const utils::Vector2f size{5, 5};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f position2{24, 23};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    std::shared_ptr<ComponentOwner> componentOwnerWithDefaultCollider1 =
        std::make_shared<ComponentOwner>(position1, "collisionSystemTest1", sharedContext);
    std::shared_ptr<ComponentOwner> componentOwnerWithDefaultCollider2 =
        std::make_shared<ComponentOwner>(position2, "collisionSystemTest2", sharedContext);
    std::shared_ptr<ComponentOwner> componentOwnerWithTileCollider1 =
        std::make_shared<ComponentOwner>(position1, "collisionSystemTest3", sharedContext);
    std::shared_ptr<ComponentOwner> componentOwnerWithTileCollider2 =
        std::make_shared<ComponentOwner>(position2, "collisionSystemTest4", sharedContext);
    std::shared_ptr<ComponentOwner> componentOwnerWithPlayerCollider1 =
        std::make_shared<ComponentOwner>(position1, "collisionSystemTest5", sharedContext);
    std::shared_ptr<ComponentOwner> componentOwnerWithPlayerCollider2 =
        std::make_shared<ComponentOwner>(position2, "collisionSystemTest6", sharedContext);
    std::shared_ptr<ComponentOwner> componentOwnerWithoutCollider =
        std::make_shared<ComponentOwner>(position2, "collisionSystemTest7", sharedContext);
    std::shared_ptr<ComponentOwner> componentOwnerWitStaticTransform =
        std::make_shared<ComponentOwner>(position2, "collisionSystemTest8", sharedContext);
    std::shared_ptr<NiceMock<animations::AnimatorMock>> animator =
        std::make_shared<NiceMock<animations::AnimatorMock>>();

    std::shared_ptr<physics::Quadtree> quadtree = std::make_shared<physics::DefaultQuadtree>();
    std::shared_ptr<physics::RayCast> rayCast = std::make_shared<physics::DefaultRayCast>(quadtree);
    DefaultCollisionSystem collisionSystem{quadtree};
};

TEST_F(DefaultCollisionSystemTest, addOwnersWithoutColliderComponent_shouldNotThrow)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwnersWithoutColliders{
        componentOwnerWithoutCollider};

    ASSERT_NO_THROW(collisionSystem.add(componentOwnersWithoutColliders));
}

TEST_F(DefaultCollisionSystemTest, addOwnersWithColliderComponent_shouldNotThrow)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwnersWithColliders{
        componentOwnerWithDefaultCollider1, componentOwnerWithDefaultCollider2,
        componentOwnerWithTileCollider1, componentOwnerWithTileCollider2, componentOwnerWithPlayerCollider2};

    ASSERT_NO_THROW(collisionSystem.add(componentOwnersWithColliders));
}

TEST_F(DefaultCollisionSystemTest, processRemovalsOnEmptyCollisionSystem_shouldNotThrow)
{
    ASSERT_NO_THROW(collisionSystem.processRemovals());
}

TEST_F(DefaultCollisionSystemTest, processRemovalsOnNonEmptyCollisionSystem_shouldNotThrow)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwnersWithColliders{
        componentOwnerWithDefaultCollider1, componentOwnerWithDefaultCollider2,
        componentOwnerWithTileCollider1, componentOwnerWithTileCollider2, componentOwnerWithPlayerCollider2};
    collisionSystem.add(componentOwnersWithColliders);

    ASSERT_NO_THROW(collisionSystem.processRemovals());
}

TEST_F(DefaultCollisionSystemTest, updateOnEmptyCollisionSystem_shouldNotThrow)
{
    ASSERT_NO_THROW(collisionSystem.update());
}

TEST_F(DefaultCollisionSystemTest,
       defaultColliderIntersectingWithDefaultCollider_shouldBlockRightAndDownMovements)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithDefaultCollider1,
                                                                 componentOwnerWithDefaultCollider2};
    collisionSystem.add(componentOwners);

    collisionSystem.update();

    ASSERT_FALSE(canMoveRight(componentOwnerWithDefaultCollider1) &&
                 canMoveDown(componentOwnerWithDefaultCollider1));
    ASSERT_TRUE(canMoveLeft(componentOwnerWithDefaultCollider1) &&
                canMoveUp(componentOwnerWithDefaultCollider1));
}

TEST_F(DefaultCollisionSystemTest, defaultColliderIntersectingWithTileCollider_shouldNotBlockAnyMovements)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithDefaultCollider1,
                                                                 componentOwnerWithTileCollider2};
    collisionSystem.add(componentOwners);

    collisionSystem.update();

    ASSERT_TRUE(canMoveLeft(componentOwners[0]) && canMoveUp(componentOwners[0]) &&
                canMoveRight(componentOwners[0]) && canMoveDown(componentOwners[0]));
}

TEST_F(DefaultCollisionSystemTest, defaultColliderIntersectingWithPlayerCollider_shouldNotBlockAnyMovements)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithDefaultCollider1,
                                                                 componentOwnerWithPlayerCollider2};
    collisionSystem.add(componentOwners);

    collisionSystem.update();

    ASSERT_TRUE(canMoveLeft(componentOwners[0]) && canMoveUp(componentOwners[0]) &&
                canMoveRight(componentOwners[0]) && canMoveDown(componentOwners[0]));
}

TEST_F(DefaultCollisionSystemTest, tileColliderIntersectingWithDefaultCollider_shouldNotBlockAnyMovements)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithTileCollider1,
                                                                 componentOwnerWithDefaultCollider2};
    collisionSystem.add(componentOwners);

    collisionSystem.update();

    ASSERT_TRUE(canMoveLeft(componentOwners[0]) && canMoveUp(componentOwners[0]) &&
                canMoveRight(componentOwners[0]) && canMoveDown(componentOwners[0]));
}

TEST_F(DefaultCollisionSystemTest, tileColliderIntersectingWithTileCollider_shouldNotBlockAnyMovements)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithTileCollider1,
                                                                 componentOwnerWithTileCollider2};
    collisionSystem.add(componentOwners);

    collisionSystem.update();

    ASSERT_TRUE(canMoveLeft(componentOwners[0]) && canMoveUp(componentOwners[0]) &&
                canMoveRight(componentOwners[0]) && canMoveDown(componentOwners[0]));
}

TEST_F(DefaultCollisionSystemTest, tileColliderIntersectingWithPlayerCollider_shouldNotBlockAnyMovements)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithTileCollider1,
                                                                 componentOwnerWithPlayerCollider2};
    collisionSystem.add(componentOwners);

    collisionSystem.update();

    ASSERT_TRUE(canMoveLeft(componentOwners[0]) && canMoveUp(componentOwners[0]) &&
                canMoveRight(componentOwners[0]) && canMoveDown(componentOwners[0]));
}

TEST_F(DefaultCollisionSystemTest,
       playerColliderIntersectingWithDefaultCollider_shouldBlockRightAndDownMovements)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithPlayerCollider1,
                                                                 componentOwnerWithDefaultCollider2};
    collisionSystem.add(componentOwners);

    collisionSystem.update();

    ASSERT_FALSE(canMoveRight(componentOwnerWithPlayerCollider1) &&
                 canMoveDown(componentOwnerWithPlayerCollider1));
    ASSERT_TRUE(canMoveLeft(componentOwnerWithPlayerCollider1) &&
                canMoveUp(componentOwnerWithPlayerCollider1));
}

TEST_F(DefaultCollisionSystemTest, playerColliderIntersectingWithTileCollider_shouldNotBlockAnyMovements)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithPlayerCollider1,
                                                                 componentOwnerWithTileCollider2};
    collisionSystem.add(componentOwners);

    collisionSystem.update();

    ASSERT_FALSE(canMoveRight(componentOwnerWithPlayerCollider1) &&
                 canMoveDown(componentOwnerWithPlayerCollider1));
    ASSERT_TRUE(canMoveLeft(componentOwnerWithPlayerCollider1) &&
                canMoveUp(componentOwnerWithPlayerCollider1));
}

TEST_F(DefaultCollisionSystemTest,
       playerColliderIntersectingWithPlayerCollider_shouldBlockRightAndDownMovements)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithPlayerCollider1,
                                                                 componentOwnerWithPlayerCollider2};
    collisionSystem.add(componentOwners);

    collisionSystem.update();

    ASSERT_FALSE(canMoveRight(componentOwnerWithPlayerCollider1) &&
                 canMoveDown(componentOwnerWithPlayerCollider1));
    ASSERT_TRUE(canMoveLeft(componentOwnerWithPlayerCollider1) &&
                canMoveUp(componentOwnerWithPlayerCollider1));
}

TEST_F(DefaultCollisionSystemTest,
       playercolliderWithStaticTransformIntersectingWithTile_shouldNotBlockAnyMovements)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWitStaticTransform,
                                                                 componentOwnerWithTileCollider2};
    collisionSystem.add(componentOwners);

    collisionSystem.update();

    ASSERT_TRUE(canMoveLeft(componentOwners[0]) && canMoveUp(componentOwners[0]) &&
                canMoveRight(componentOwners[0]) && canMoveDown(componentOwners[0]));
}

TEST_F(DefaultCollisionSystemTest, givenTwoSameColliders_shouldNotBlockAnyMovements)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithDefaultCollider1,
                                                                 componentOwnerWithDefaultCollider1};
    collisionSystem.add(componentOwners);

    collisionSystem.update();

    ASSERT_TRUE(canMoveLeft(componentOwners[0]) && canMoveUp(componentOwners[0]) &&
                canMoveRight(componentOwners[0]) && canMoveDown(componentOwners[0]));
}
