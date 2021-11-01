#include "CollectableItemComponent.h"

#include "gtest/gtest.h"

#include "ComponentOwner.h"
#include "HealthComponent.h"

using namespace components::core;
using namespace ::testing;

class CollectableItemComponentTest : public Test
{
public:
    CollectableItemComponentTest()
    {
        collector.addComponent<HealthComponent>(initialHealthPoints);
        collector.loadDependentComponents();
    }

    const utils::Vector2f position{20, 20};
    const std::string itemName{"name"};
    ComponentOwner owner{position, "CollectableItemComponentTest1"};
    const unsigned int initialHealthPoints{100};
    ComponentOwner collector{position, "CollectableItemComponentTest2"};
    CollectableItemComponent collectableItem{&owner, itemName};
};

TEST_F(CollectableItemComponentTest, collect_shouldDisableComponents)
{
    collectableItem.collectBy(&collector);

    ASSERT_FALSE(owner.areComponentsEnabled());
}

TEST_F(CollectableItemComponentTest, drop_shouldEnableComponentsGraphicAndPhysics)
{
    collectableItem.drop();

    ASSERT_TRUE(owner.areComponentsEnabled());
}

TEST_F(CollectableItemComponentTest, use_shouldGive1HealthPointToCollectorAndRemoveItself)
{
    const auto health = collector.getComponent<HealthComponent>();
    const auto lostHealthPoints = 5;
    health->loseHealthPoints(lostHealthPoints);
    collectableItem.collectBy(&collector);

    collectableItem.use();

    ASSERT_TRUE(owner.shouldBeRemoved());
    ASSERT_EQ(health->getCurrentHealth(), initialHealthPoints - lostHealthPoints + 1);
}

TEST_F(CollectableItemComponentTest, shouldReturnName)
{
    ASSERT_EQ(collectableItem.getName(), itemName);
}
