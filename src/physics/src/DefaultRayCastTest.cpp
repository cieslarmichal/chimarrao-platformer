#include "DefaultRayCast.h"

#include "gtest/gtest.h"

#include "DefaultQuadtree.h"
#include "RendererPoolMock.h"

using namespace physics;
using namespace components::core;
using namespace ::testing;

class DefaultRayCastTest : public Test
{
public:
    const utils::Vector2f size{5, 5};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f position1Center{22.5, 22.5};
    const utils::Vector2f targetPoint1{27, 22.5};
    const utils::Vector2f targetPoint2{27, 22.5};
    const utils::Vector2f position2{26, 20};
    const utils::Vector2f position3{32, 20};
    const utils::Vector2f position4{40, 20};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner1{position1, "RayCastTest1", sharedContext};
    ComponentOwner componentOwner2{position2, "RayCastTest2", sharedContext};
    ComponentOwner componentOwner3{position3, "RayCastTest3", sharedContext};
    ComponentOwner componentOwner4{position4, "RayCastTest4", sharedContext};
    std::shared_ptr<BoxColliderComponent> boxColliderComponent1 =
        std::make_shared<BoxColliderComponent>(&componentOwner1, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent2 =
        std::make_shared<BoxColliderComponent>(&componentOwner2, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent3 =
        std::make_shared<BoxColliderComponent>(&componentOwner3, size);
    std::shared_ptr<BoxColliderComponent> boxColliderComponent4 =
        std::make_shared<BoxColliderComponent>(&componentOwner4, size);

    std::shared_ptr<physics::DefaultQuadtree> quadtree = std::make_shared<physics::DefaultQuadtree>();
    DefaultRayCast rayCast{quadtree};
};

TEST_F(DefaultRayCastTest, givenOneNotInRange_shouldReturnNoObject)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponent4);

    const auto rayCastResult =
        rayCast.cast(position1Center, targetPoint1, boxColliderComponent1->getOwnerId());

    ASSERT_FALSE(rayCastResult.collision);
}

TEST_F(DefaultRayCastTest, givenOneObjectInRangeOnRight_shouldReturnIntersectedObject)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponent2);

    const auto rayCastResult =
        rayCast.cast(position1Center, targetPoint1, boxColliderComponent1->getOwnerId());

    ASSERT_EQ(rayCastResult.collision, &(boxColliderComponent2->getOwner()));
}

TEST_F(DefaultRayCastTest, givenTwoObjectsInRangeOnRight_shouldReturnFirstIntersectingOne)
{
    quadtree->insertCollider(boxColliderComponent1);
    quadtree->insertCollider(boxColliderComponent2);
    quadtree->insertCollider(boxColliderComponent3);

    const auto rayCastResult =
        rayCast.cast(position1Center, targetPoint2, boxColliderComponent1->getOwnerId());

    ASSERT_EQ(rayCastResult.collision, &(boxColliderComponent2->getOwner()));
}
