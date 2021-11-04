#include "ItemCollectorComponent.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "ItemEffectMock.h"

#include "ComponentOwner.h"
#include "HealthComponent.h"
#include "core/exceptions/DependentComponentNotFound.h"
#include "core/exceptions/InvalidCapacity.h"

using namespace components::core;
using namespace ::testing;

class ItemCollectorComponentTest : public Test
{
public:
    ItemCollectorComponentTest()
    {
        itemCollectorOwner.addComponent<HealthComponent>(initialHealthPoints);
        const auto velocity = itemCollectorOwner.addComponent<VelocityComponent>();
        velocity->setVelocity(5,0);
        itemCollectorOwner.addComponent<DirectionComponent>();
        itemCollectorOwner.addComponent<AnimationComponent>(animator);
        boxColliderComponent = itemCollectorOwner.addComponent<BoxColliderComponent>(size);
        itemCollectorWithOneCapacity.loadDependentComponents();
        itemCollectorWithTwoCapacity.loadDependentComponents();

        boxColliderComponent1 = itemOwner1.addComponent<BoxColliderComponent>(size);
        itemOwner1.addComponent<CollectableItemComponent>(itemName1, itemType, itemEffect);
        itemOwner1.loadDependentComponents();

        boxColliderComponent2 = itemOwner2.addComponent<BoxColliderComponent>(size);
        itemOwner2.addComponent<CollectableItemComponent>(itemName2, itemType, itemEffect);
        itemOwner2.loadDependentComponents();

        boxColliderComponent3 = itemOwner3.addComponent<BoxColliderComponent>(size);
        itemOwner3.addComponent<CollectableItemComponent>(itemName3, itemType, itemEffect);
        itemOwner3.loadDependentComponents();
    }

    const unsigned emptyCapacity{0};
    const unsigned capacity1{1};
    const unsigned capacity2{2};
    const utils::Vector2f size{4, 4};
    const utils::Vector2f position{0, 0};
    const utils::Vector2f position1{5, 0};
    const utils::Vector2f position2{2, 0};
    const utils::Vector2f position3{3, 0};
    const std::string itemName1{"item1"};
    const std::string itemName2{"item2"};
    const std::string itemName3{"item3"};
    const ItemType itemType{ItemType::Apple};
    const std::string nonExistingItemName{"nonExistingItemName"};
    ComponentOwner itemCollectorOwner{position, "ItemCollectorComponentTest1"};
    const unsigned int initialHealthPoints{100};
    ComponentOwner itemOwner1{position1, "ItemCollectorComponentTest2"};
    ComponentOwner itemOwner2{position2, "ItemCollectorComponentTest3"};
    ComponentOwner itemOwner3{position3, "ItemCollectorComponentTest4"};
    std::shared_ptr<BoxColliderComponent> boxColliderComponent;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent1;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent2;
    std::shared_ptr<BoxColliderComponent> boxColliderComponent3;
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<StrictMock<ItemEffectMock>> itemEffect = std::make_shared<StrictMock<ItemEffectMock>>();
    std::shared_ptr<physics::Quadtree> quadtree = std::make_shared<physics::Quadtree>();
    std::shared_ptr<physics::RayCast> rayCast = std::make_shared<physics::RayCast>(quadtree);
    ItemCollectorComponent itemCollectorWithOneCapacity{&itemCollectorOwner, quadtree, rayCast, capacity1};
    ItemCollectorComponent itemCollectorWithTwoCapacity{&itemCollectorOwner, quadtree, rayCast, capacity2};
};

TEST_F(ItemCollectorComponentTest, givenZeroCapacity_shouldThrowInvalidCapacityException)
{
    ASSERT_THROW(ItemCollectorComponent(&itemCollectorOwner, quadtree, rayCast, emptyCapacity),
                 exceptions::InvalidCapacity);
}

TEST_F(ItemCollectorComponentTest,
       loadDependentComponentsWithoutDirectionComponent_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutDirection{position, "componentOwnerWithoutDirection"};
    ItemCollectorComponent itemCollectorWithoutDirection{&componentOwnerWithoutDirection, quadtree, rayCast,
                                                         capacity1};

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
                                                           rayCast, capacity1};

    ASSERT_THROW(itemCollectorWithoutBoxCollider.loadDependentComponents(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(ItemCollectorComponentTest, givenItemOutOfRange_shouldNotCollectItem)
{
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent1);

    itemCollectorWithOneCapacity.collectNearestItem();

    ASSERT_TRUE(itemCollectorWithOneCapacity.getItems().empty());
}

TEST_F(ItemCollectorComponentTest, givenTwoItemsToCollectAndOnlyOneCollectorCapacity_shouldCollectOnlyOneItem)
{
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent2);
    quadtree->insertCollider(boxColliderComponent3);

    itemCollectorWithOneCapacity.collectNearestItem();
    itemCollectorWithOneCapacity.collectNearestItem();

    const auto items = itemCollectorWithOneCapacity.getItems();
    ASSERT_EQ(items.size(), 1);
    ASSERT_EQ(items[0]->getName(), itemName2);
}

TEST_F(ItemCollectorComponentTest, givenItems_shouldCollectTwoClosestItems)
{
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent2);
    quadtree->insertCollider(boxColliderComponent3);

    itemCollectorWithTwoCapacity.collectNearestItem();
    itemCollectorWithTwoCapacity.collectNearestItem();

    const auto items = itemCollectorWithTwoCapacity.getItems();
    ASSERT_EQ(items.size(), 2);
    ASSERT_EQ(items[0]->getName(), itemName2);
    ASSERT_EQ(items[1]->getName(), itemName3);
}

TEST_F(ItemCollectorComponentTest, givenNonExisitingItemName_shouldNotDropAnyItem)
{
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent2);
    quadtree->insertCollider(boxColliderComponent3);
    itemCollectorWithTwoCapacity.collectNearestItem();
    itemCollectorWithTwoCapacity.collectNearestItem();

    itemCollectorWithTwoCapacity.drop(nonExistingItemName);

    const auto items = itemCollectorWithTwoCapacity.getItems();
    ASSERT_EQ(items.size(), 2);
}

TEST_F(ItemCollectorComponentTest, givenExistingItemButNoPlaceToDropItem_shouldNotDropItem)
{
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent2);
    quadtree->insertCollider(boxColliderComponent3);
    itemCollectorWithTwoCapacity.collectNearestItem();

    itemCollectorWithTwoCapacity.drop(itemName2);

    const auto items = itemCollectorWithTwoCapacity.getItems();
    ASSERT_EQ(items.size(), 1);
}

TEST_F(ItemCollectorComponentTest,
       givenExistingItemAndValidPlaceToDropItem_shouldSetDropPositionToItemAndDeleteItemFromCollection)
{
    const utils::Vector2f expectedDropPlace{6, 2};
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent2);
    itemCollectorWithTwoCapacity.collectNearestItem();

    itemCollectorWithTwoCapacity.drop(itemName2);

    const auto items = itemCollectorWithTwoCapacity.getItems();
    ASSERT_EQ(items.size(), 0);
    ASSERT_EQ(itemOwner2.transform->getPosition(), expectedDropPlace);
}

TEST_F(ItemCollectorComponentTest, givenNonExisitingItemName_shouldNotUseAnyItem)
{
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent2);
    quadtree->insertCollider(boxColliderComponent3);
    itemCollectorWithTwoCapacity.collectNearestItem();
    itemCollectorWithTwoCapacity.collectNearestItem();

    itemCollectorWithTwoCapacity.use(nonExistingItemName);

    const auto items = itemCollectorWithTwoCapacity.getItems();
    ASSERT_EQ(items.size(), 2);
}

TEST_F(ItemCollectorComponentTest, givenExistingItem_shouldInvokeItemEffectAndDeleteItemFromCollection)
{
    quadtree->insertCollider(boxColliderComponent);
    quadtree->insertCollider(boxColliderComponent2);
    itemCollectorWithTwoCapacity.collectNearestItem();
    EXPECT_CALL(*itemEffect, affect(&itemCollectorOwner));

    itemCollectorWithTwoCapacity.use(itemName2);

    const auto items = itemCollectorWithTwoCapacity.getItems();
    ASSERT_EQ(items.size(), 0);
}

