#include "Quadtree.h"

#include "gtest/gtest.h"

#include "StlOperators.h"

using namespace physics;
using namespace components::core;
using namespace ::testing;

class QuadtreeTest : public Test
{
public:
    const utils::Vector2f size{5, 5};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f position2{22, 22};
    const utils::Vector2f position3{30, 30};
    const utils::Vector2f position4{30.1f, 30.1f};
    const utils::Vector2f position5{35, 35};
    const utils::Vector2f position6{43, 33};
    const utils::Vector2f position7{45, 20};
    const utils::Vector2f position8{60, 15};
    const utils::Vector2f position9{60, 65};
    const utils::Vector2f position10{20, 50};
    const utils::Vector2f positionOnNorthEdge{38, 10};
    const utils::Vector2f positionOnEastEdge{60, 28};
    const utils::Vector2f positionOnSouthEdge{38, 50};
    const utils::Vector2f positionOnWestEdge{20, 28};
    const utils::FloatRect area1{18, 18, 5, 5};
    const utils::FloatRect area2{28, 28, 4, 4};
    const utils::FloatRect area3{60, 25, 10, 5};
    ComponentOwner componentOwner1{position1, "quadtreeTest1"};
    ComponentOwner componentOwner2{position2, "quadtreeTest2"};
    ComponentOwner componentOwner3{position3, "quadtreeTest3"};
    ComponentOwner componentOwner4{position4, "quadtreeTest4"};
    ComponentOwner componentOwner5{position5, "quadtreeTest5"};
    ComponentOwner componentOwner6{position6, "quadtreeTest6"};
    ComponentOwner componentOwner7{position7, "quadtreeTest7"};
    ComponentOwner componentOwner8{position8, "quadtreeTest8"};
    ComponentOwner componentOwner9{position9, "quadtreeTest9"};
    ComponentOwner componentOwner10{position10, "quadtreeTest10"};
    ComponentOwner componentOwnerOnNorthEdge{positionOnNorthEdge, "componentOwnerOnNorthEdge"};
    ComponentOwner componentOwnerOnEastEdge{positionOnEastEdge, "componentOwnerOnEastEdge"};
    ComponentOwner componentOwnerOnSouthEdge{positionOnSouthEdge, "componentOwnerOnSouthEdge"};
    ComponentOwner componentOwnerOnWestEdge{positionOnWestEdge, "componentOwnerOnWestEdge"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponent1 =
        std::make_shared<BoxColliderComponent>(&componentOwner1, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent2 =
        std::make_shared<BoxColliderComponent>(&componentOwner2, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent3 =
        std::make_shared<BoxColliderComponent>(&componentOwner3, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent4 =
        std::make_shared<BoxColliderComponent>(&componentOwner4, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent5 =
        std::make_shared<BoxColliderComponent>(&componentOwner5, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent6 =
        std::make_shared<BoxColliderComponent>(&componentOwner6, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent7 =
        std::make_shared<BoxColliderComponent>(&componentOwner7, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent8 =
        std::make_shared<BoxColliderComponent>(&componentOwner8, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent9 =
        std::make_shared<BoxColliderComponent>(&componentOwner9, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent10 =
        std::make_shared<BoxColliderComponent>(&componentOwner10, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponentOnNorthEdge =
        std::make_shared<BoxColliderComponent>(&componentOwnerOnNorthEdge, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponentOnEastEdge =
        std::make_shared<BoxColliderComponent>(&componentOwnerOnEastEdge, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponentOnSouthEdge =
        std::make_shared<BoxColliderComponent>(&componentOwnerOnSouthEdge, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponentOnWestEdge =
        std::make_shared<BoxColliderComponent>(&componentOwnerOnWestEdge, size);
};

TEST_F(QuadtreeTest, insertedColliderIntersectingWithArea_canBeIntersectedWithArea)
{
    Quadtree quadtree{};
    quadtree.insertCollider(boxColliderComponent1);

    const auto collidersIntersectingWithArea = quadtree.getCollidersIntersectingWithAreaFromX(area1);

    ASSERT_TRUE(std::find(collidersIntersectingWithArea.begin(), collidersIntersectingWithArea.end(),
                          boxColliderComponent1) != collidersIntersectingWithArea.end());
}

TEST_F(QuadtreeTest, removedColliderIntersectingWithArea_canNotBeIntersectedWithArea)
{
    Quadtree quadtree{};
    quadtree.insertCollider(boxColliderComponent1);
    quadtree.removeCollider(boxColliderComponent1);

    const auto collidersIntersectingWithArea = quadtree.getCollidersIntersectingWithAreaFromX(area1);

    ASSERT_TRUE(collidersIntersectingWithArea.empty());
}

TEST_F(QuadtreeTest, clearedCollidersIntersectingWithArea_shouldReturnEmptyColliders)
{
    Quadtree quadtree{};
    quadtree.insertCollider(boxColliderComponent1);
    quadtree.insertCollider(boxColliderComponent2);
    quadtree.clearAllColliders();

    const auto collidersIntersectingWithArea = quadtree.getCollidersIntersectingWithAreaFromX(area1);

    ASSERT_TRUE(collidersIntersectingWithArea.empty());
}

TEST_F(QuadtreeTest, clearCollidersFromDifferentQuadtreeChildren_shouldReturnEmptyColliders)
{
    Quadtree quadtree{2, 10, 1, utils::FloatRect(0, 0, 80, 60)};
    quadtree.insertCollider(boxColliderComponent1);
    quadtree.insertCollider(boxColliderComponent2);
    quadtree.insertCollider(boxColliderComponent3);
    quadtree.insertCollider(boxColliderComponent4);
    quadtree.insertCollider(boxColliderComponent5);
    quadtree.insertCollider(boxColliderComponent6);
    quadtree.insertCollider(boxColliderComponent7);
    quadtree.insertCollider(boxColliderComponent8);
    quadtree.insertCollider(boxColliderComponent9);
    quadtree.insertCollider(boxColliderComponent10);

    quadtree.clearAllColliders();

    const auto collidersIntersectingWithArea = quadtree.getCollidersIntersectingWithAreaFromX(area1);
    ASSERT_TRUE(collidersIntersectingWithArea.empty());
}

TEST_F(QuadtreeTest, givenNoColliders_shouldReturnEmptyColliders)
{
    Quadtree quadtree{};
    const auto collidersIntersectingWithArea = quadtree.getCollidersIntersectingWithAreaFromX(area1);

    ASSERT_TRUE(collidersIntersectingWithArea.empty());
}

TEST_F(QuadtreeTest, givenColliderNotIntersectingWithArea_shouldReturnEmptyColliders)
{
    Quadtree quadtree{};
    quadtree.insertCollider(boxColliderComponent3);

    const auto collidersIntersectingWithArea = quadtree.getCollidersIntersectingWithAreaFromX(area1);

    ASSERT_TRUE(collidersIntersectingWithArea.empty());
}

TEST_F(QuadtreeTest, givenTwoColliders_onlyOneOfThemIntersectingWithArea_shouldReturnOneCollider)
{
    Quadtree quadtree{};
    quadtree.insertCollider(boxColliderComponent1);
    quadtree.insertCollider(boxColliderComponent3);

    const auto collidersIntersectingWithArea = quadtree.getCollidersIntersectingWithAreaFromX(area1);

    ASSERT_EQ(collidersIntersectingWithArea.size(), 1u);
    ASSERT_TRUE(std::find(collidersIntersectingWithArea.begin(), collidersIntersectingWithArea.end(),
                          boxColliderComponent1) != collidersIntersectingWithArea.end());
}

TEST_F(
    QuadtreeTest,
    givenTenCollidersAtDifferentPlaces_andAreaIntersectingWithThem_shouldReturnCollidersIntersectingWithArea)
{
    Quadtree quadtree{2, 10, 1, utils::FloatRect(0, 0, 80, 60)};
    const std::vector<std::shared_ptr<BoxColliderComponent>> expectedColliders{boxColliderComponent3,
                                                                               boxColliderComponent4};
    quadtree.insertCollider(boxColliderComponent1);
    quadtree.insertCollider(boxColliderComponent2);
    quadtree.insertCollider(boxColliderComponent3);
    quadtree.insertCollider(boxColliderComponent4);
    quadtree.insertCollider(boxColliderComponent5);
    quadtree.insertCollider(boxColliderComponent6);
    quadtree.insertCollider(boxColliderComponent7);
    quadtree.insertCollider(boxColliderComponent8);
    quadtree.insertCollider(boxColliderComponent9);
    quadtree.insertCollider(boxColliderComponent10);

    const auto collidersIntersectingWithArea = quadtree.getCollidersIntersectingWithAreaFromX(area2);

    ASSERT_TRUE(compareVectors(collidersIntersectingWithArea, expectedColliders));
}

TEST_F(QuadtreeTest,
       givenTenCollidersAtDifferentPlaces_andAreaNotIntersectingWithThem_shouldReturnEmptyColliders)
{
    Quadtree quadtree{};
    quadtree.insertCollider(boxColliderComponent1);
    quadtree.insertCollider(boxColliderComponent2);
    quadtree.insertCollider(boxColliderComponent3);
    quadtree.insertCollider(boxColliderComponent4);
    quadtree.insertCollider(boxColliderComponent5);
    quadtree.insertCollider(boxColliderComponent6);
    quadtree.insertCollider(boxColliderComponent7);
    quadtree.insertCollider(boxColliderComponent8);
    quadtree.insertCollider(boxColliderComponent9);
    quadtree.insertCollider(boxColliderComponent10);

    const auto collidersIntersectingWithArea = quadtree.getCollidersIntersectingWithAreaFromX(area3);

    ASSERT_TRUE(collidersIntersectingWithArea.empty());
}

TEST_F(QuadtreeTest, shouldReturnDefaultNodeBounds)
{
    Quadtree quadtree{};

    const auto nodeBounds = quadtree.getNodeBounds();

    ASSERT_EQ(nodeBounds, utils::FloatRect(0, 0, 160, 60));
}

TEST_F(QuadtreeTest, shouldReturnNodeBoundsSetInContructor)
{
    const auto expectedBounds = utils::FloatRect(5, 5, 50, 50);
    Quadtree quadtree{5, 6, 1, expectedBounds};

    const auto actualNodeBounds = quadtree.getNodeBounds();

    ASSERT_EQ(actualNodeBounds, expectedBounds);
}

TEST_F(QuadtreeTest, giveCollidersOnEdgesAndRandoms_requestColidersFromScreenArea_shouldReturnAllColliders)
{
    const auto boxColliders = {
        boxColliderComponent1,          boxColliderComponent2,          boxColliderComponent3,
        boxColliderComponent4,          boxColliderComponent5,          boxColliderComponent6,
        boxColliderComponent7,          boxColliderComponent8,          boxColliderComponent9,
        boxColliderComponent10,         boxColliderComponentOnEastEdge, boxColliderComponentOnSouthEdge,
        boxColliderComponentOnWestEdge, boxColliderComponentOnNorthEdge};
    Quadtree quadtree{};
    for (const auto& boxCollider : boxColliders)
    {
        quadtree.insertCollider(boxCollider);
    }

    for (const auto& boxCollider : boxColliders)
    {
        auto box = boxCollider->getCollisionBox();
        box.left += 0.1f;
        box.top += 0.1f;
        const auto collidersIntersectingWithArea = quadtree.getCollidersIntersectingWithAreaFromX(box);

        ASSERT_GE(collidersIntersectingWithArea.size(), 1);
    }
}