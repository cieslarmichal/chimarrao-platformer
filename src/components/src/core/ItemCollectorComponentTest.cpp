#include "ItemCollectorComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"

#include "ComponentOwner.h"
#include "HealthComponent.h"
#include "core/exceptions/DependentComponentNotFound.h"

using namespace components::core;
using namespace ::testing;

class ItemCollectorComponentTest : public Test
{
public:
    ItemCollectorComponentTest()
    {
        itemCollectorOwner.addComponent<HealthComponent>(initialHealthPoints);
        itemCollectorOwner.addComponent<VelocityComponent>();
        itemCollectorOwner.addComponent<DirectionComponent>();
        itemCollectorOwner.addComponent<AnimationComponent>(animator);
        boxColliderComponent = itemCollectorOwner.addComponent<BoxColliderComponent>(size);
        itemCollector.loadDependentComponents();

        boxColliderComponent1 = itemOwner1.addComponent<BoxColliderComponent>(size);
        itemOwner1.addComponent<CollectableItemComponent>(itemName1);
        itemOwner1.loadDependentComponents();

        boxColliderComponent2 = itemOwner2.addComponent<BoxColliderComponent>(size);
        itemOwner2.addComponent<CollectableItemComponent>(itemName2);
        itemOwner2.loadDependentComponents();

        boxColliderComponent3 = itemOwner3.addComponent<BoxColliderComponent>(size);
        itemOwner3.addComponent<CollectableItemComponent>(itemName3);
        itemOwner3.loadDependentComponents();
    }

    const utils::Vector2f size{4, 4};
    const utils::Vector2f position{0, 0};
    const utils::Vector2f position1{5, 0};
    const utils::Vector2f position2{4, 0};
    const utils::Vector2f position3{4.5, 0};
    const std::string itemName1{"item1"};
    const std::string itemName2{"item2"};
    const std::string itemName3{"item3"};
    ComponentOwner itemCollectorOwner{position, "ItemCollectorComponentTest1"};
    const unsigned int initialHealthPoints{100};
    ComponentOwner itemOwner1{position1, "ItemCollectorComponentTest2"};
    ComponentOwner itemOwner2{position2, "ItemCollectorComponentTest3"};
    ComponentOwner itemOwner3{position3, "ItemCollectorComponentTest4"};
    std::shared_ptr<CollectableItemComponent> collectableItem1;
    std::shared_ptr<CollectableItemComponent> collectableItem2;
    std::shared_ptr<CollectableItemComponent> collectableItem3;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent1;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent2;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent3;
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<physics::Quadtree> quadtree = std::make_shared<physics::Quadtree>();
    std::shared_ptr<physics::RayCast> rayCast = std::make_shared<physics::RayCast>(quadtree);
    ItemCollectorComponent itemCollector{&itemCollectorOwner, quadtree, rayCast};
};

TEST_F(ItemCollectorComponentTest,
       loadDependentComponentsWithoutDirectionComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutDirection{position, "componentOwnerWithoutDirection"};
    ItemCollectorComponent itemCollectorWithoutDirection{&componentOwnerWithoutDirection, quadtree, rayCast};

    ASSERT_THROW(itemCollectorWithoutDirection.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(ItemCollectorComponentTest,
       loadDependentComponentsWithoutBoxColliderComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutBoxCollider{position, "componentOwnerWithoutBoxCollider"};
    componentOwnerWithoutBoxCollider.addComponent<VelocityComponent>();
    componentOwnerWithoutBoxCollider.addComponent<DirectionComponent>();
    ItemCollectorComponent itemCollectorWithoutBoxCollider{&componentOwnerWithoutBoxCollider, quadtree,
                                                           rayCast};

    ASSERT_THROW(itemCollectorWithoutBoxCollider.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(ItemCollectorComponentTest, givenItemOutOfRange_shouldNotCollectItem)
{
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent1);

    itemCollector.collectNearestItem();

    ASSERT_TRUE(itemCollector.getItems().empty());
}

 TEST_F(ItemCollectorComponentTest, givenItems_shouldCollectClosestItem)
{
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent2);
    quadtree->insertCollider(boxColliderComponent3);

    itemCollector.collectNearestItem();

    const auto items = itemCollector.getItems();
    ASSERT_EQ(items.size(), 1);
    ASSERT_EQ(items[0]->getName(), itemName2);
 }
//
// TEST_F(ItemCollectorComponentTest, drop_shouldEnableComponentsGraphicAndPhysics)
//{
//     collectableItem.drop();
//
//     ASSERT_TRUE(owner.areComponentsEnabled());
// }
//
// TEST_F(ItemCollectorComponentTest, use_shouldGive1HealthPointToCollectorAndRemoveItself)
//{
//     const auto health = collector.getComponent<HealthComponent>();
//     const auto lostHealthPoints = 5;
//     health->loseHealthPoints(lostHealthPoints);
//     collectableItem.collectBy(&collector);
//
//     collectableItem.use();
//
//     ASSERT_TRUE(owner.shouldBeRemoved());
//     ASSERT_EQ(health->getCurrentHealth(), initialHealthPoints - lostHealthPoints + 1);
// }
//
// TEST_F(ItemCollectorComponentTest, shouldReturnName)
//{
//     ASSERT_EQ(collectableItem.getName(), itemName);
// }
