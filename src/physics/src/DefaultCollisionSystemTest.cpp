#include "DefaultCollisionSystem.h"

#include "gtest/gtest.h"

#include "StlOperators.h"

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
        componentOwnerWitStaticTransform->transform->setStaticTransform(true);
    }

    const utils::Vector2f size{5, 5};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f position2{24, 23};
    std::shared_ptr<ComponentOwner> componentOwnerWithDefaultCollider1 =
        std::make_shared<ComponentOwner>(position1, "collisionSystemTest1");
    std::shared_ptr<ComponentOwner> componentOwnerWithDefaultCollider2 =
        std::make_shared<ComponentOwner>(position2, "collisionSystemTest2");
    std::shared_ptr<ComponentOwner> componentOwnerWithTileCollider1 =
        std::make_shared<ComponentOwner>(position1, "collisionSystemTest3");
    std::shared_ptr<ComponentOwner> componentOwnerWithTileCollider2 =
        std::make_shared<ComponentOwner>(position2, "collisionSystemTest4");
    std::shared_ptr<ComponentOwner> componentOwnerWithPlayerCollider1 =
        std::make_shared<ComponentOwner>(position1, "collisionSystemTest5");
    std::shared_ptr<ComponentOwner> componentOwnerWithPlayerCollider2 =
        std::make_shared<ComponentOwner>(position2, "collisionSystemTest6");
    std::shared_ptr<ComponentOwner> componentOwnerWithoutCollider =
        std::make_shared<ComponentOwner>(position2, "collisionSystemTest7");
    std::shared_ptr<ComponentOwner> componentOwnerWitStaticTransform =
        std::make_shared<ComponentOwner>(position2, "collisionSystemTest8");

    DefaultCollisionSystem collisionSystem{};
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
       defaultColliderIntersectingWithDefaultCollider_defaultColliderPositionShouldBeUpdated)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithDefaultCollider1,
                                                                 componentOwnerWithDefaultCollider2};
    collisionSystem.add(componentOwners);
    const auto defaultColliderPositionBeforeUpdate =
        componentOwnerWithDefaultCollider1->transform->getPosition();

    collisionSystem.update();

    const auto defaultColliderPositionAfterUpdate =
        componentOwnerWithDefaultCollider1->transform->getPosition();
    ASSERT_NE(defaultColliderPositionBeforeUpdate, defaultColliderPositionAfterUpdate);
}

TEST_F(DefaultCollisionSystemTest,
       defaultColliderIntersectingWithTileCollider_defaultColliderPositionShouldNotBeUpdated)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithDefaultCollider1,
                                                                 componentOwnerWithTileCollider2};
    collisionSystem.add(componentOwners);
    const auto defaultColliderPositionBeforeUpdate =
        componentOwnerWithDefaultCollider1->transform->getPosition();

    collisionSystem.update();

    const auto defaultColliderPositionAfterUpdate =
        componentOwnerWithDefaultCollider1->transform->getPosition();
    ASSERT_EQ(defaultColliderPositionBeforeUpdate, defaultColliderPositionAfterUpdate);
}

TEST_F(DefaultCollisionSystemTest,
       defaultColliderIntersectingWithPlayerCollider_defaultColliderPositionShouldNotBeUpdated)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithDefaultCollider1,
                                                                 componentOwnerWithPlayerCollider2};
    collisionSystem.add(componentOwners);
    const auto defaultColliderPositionBeforeUpdate =
        componentOwnerWithDefaultCollider1->transform->getPosition();

    collisionSystem.update();

    const auto defaultColliderPositionAfterUpdate =
        componentOwnerWithDefaultCollider1->transform->getPosition();
    ASSERT_EQ(defaultColliderPositionBeforeUpdate, defaultColliderPositionAfterUpdate);
}

TEST_F(DefaultCollisionSystemTest,
       tileColliderIntersectingWithDefaultCollider_tileColliderPositionShouldNotBeUpdated)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithTileCollider1,
                                                                 componentOwnerWithDefaultCollider2};
    collisionSystem.add(componentOwners);
    const auto tileColliderPositionBeforeUpdate = componentOwnerWithTileCollider1->transform->getPosition();

    collisionSystem.update();

    const auto tileColliderPositionAfterUpdate = componentOwnerWithTileCollider1->transform->getPosition();
    ASSERT_EQ(tileColliderPositionBeforeUpdate, tileColliderPositionAfterUpdate);
}

TEST_F(DefaultCollisionSystemTest,
       tileColliderIntersectingWithTileCollider_tileColliderPositionShouldNotBeUpdated)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithTileCollider1,
                                                                 componentOwnerWithTileCollider2};
    collisionSystem.add(componentOwners);
    const auto tileColliderPositionBeforeUpdate = componentOwnerWithTileCollider1->transform->getPosition();

    collisionSystem.update();

    const auto tileColliderPositionAfterUpdate = componentOwnerWithTileCollider1->transform->getPosition();
    ASSERT_EQ(tileColliderPositionBeforeUpdate, tileColliderPositionAfterUpdate);
}

TEST_F(DefaultCollisionSystemTest,
       tileColliderIntersectingWithPlayerCollider_tileColliderPositionShouldNotBeUpdated)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithTileCollider1,
                                                                 componentOwnerWithPlayerCollider2};
    collisionSystem.add(componentOwners);
    const auto tileColliderPositionBeforeUpdate = componentOwnerWithTileCollider1->transform->getPosition();

    collisionSystem.update();

    const auto tileColliderPositionAfterUpdate = componentOwnerWithTileCollider1->transform->getPosition();
    ASSERT_EQ(tileColliderPositionBeforeUpdate, tileColliderPositionAfterUpdate);
}

TEST_F(DefaultCollisionSystemTest,
       playerColliderIntersectingWithDefaultCollider_playerColliderPositionShouldBeUpdated)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithPlayerCollider1,
                                                                 componentOwnerWithDefaultCollider1};
    collisionSystem.add(componentOwners);
    const auto playerColliderPositionBeforeUpdate =
        componentOwnerWithPlayerCollider1->transform->getPosition();

    collisionSystem.update();

    const auto playerColliderPositionAfterUpdate =
        componentOwnerWithPlayerCollider1->transform->getPosition();
    ASSERT_NE(playerColliderPositionBeforeUpdate, playerColliderPositionAfterUpdate);
}

TEST_F(DefaultCollisionSystemTest,
       playerColliderIntersectingWithTileCollider_playerColliderPositionShouldBeUpdated)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithPlayerCollider1,
                                                                 componentOwnerWithTileCollider2};
    collisionSystem.add(componentOwners);
    const auto playerColliderPositionBeforeUpdate =
        componentOwnerWithPlayerCollider1->transform->getPosition();

    collisionSystem.update();

    const auto playerColliderPositionAfterUpdate =
        componentOwnerWithPlayerCollider1->transform->getPosition();
    ASSERT_NE(playerColliderPositionBeforeUpdate, playerColliderPositionAfterUpdate);
}

TEST_F(DefaultCollisionSystemTest,
       playerColliderIntersectingWithPlayerCollider_playerColliderPositionShouldNotBeUpdated)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithPlayerCollider1,
                                                                 componentOwnerWithPlayerCollider2};
    collisionSystem.add(componentOwners);
    const auto playerColliderPositionBeforeUpdate =
        componentOwnerWithPlayerCollider1->transform->getPosition();

    collisionSystem.update();

    const auto playerColliderPositionAfterUpdate =
        componentOwnerWithPlayerCollider1->transform->getPosition();
    ASSERT_EQ(playerColliderPositionBeforeUpdate, playerColliderPositionAfterUpdate);
}

TEST_F(DefaultCollisionSystemTest,
       playercolliderWithStaticTransformIntersectingWithTile_playerPositionShouldNotBeUpdated)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWitStaticTransform,
                                                                 componentOwnerWithTileCollider2};
    collisionSystem.add(componentOwners);
    const auto playerColliderPositionBeforeUpdate =
        componentOwnerWitStaticTransform->transform->getPosition();

    collisionSystem.update();

    const auto playerColliderPositionAfterUpdate = componentOwnerWitStaticTransform->transform->getPosition();
    ASSERT_EQ(playerColliderPositionBeforeUpdate, playerColliderPositionAfterUpdate);
}

TEST_F(DefaultCollisionSystemTest, givenTwoSameColliders_shouldNotUpdatePosition)
{
    std::vector<std::shared_ptr<ComponentOwner>> componentOwners{componentOwnerWithDefaultCollider1,
                                                                 componentOwnerWithDefaultCollider1};
    collisionSystem.add(componentOwners);
    const auto defaultColliderPositionBeforeUpdate =
        componentOwnerWithDefaultCollider1->transform->getPosition();

    collisionSystem.update();

    const auto defaultColliderPositionAfterUpdate =
        componentOwnerWithDefaultCollider1->transform->getPosition();
    ASSERT_EQ(defaultColliderPositionBeforeUpdate, defaultColliderPositionAfterUpdate);
}
