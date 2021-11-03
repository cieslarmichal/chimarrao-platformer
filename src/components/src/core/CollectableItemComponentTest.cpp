#include "CollectableItemComponent.h"

#include "gtest/gtest.h"

#include "ItemEffectMock.h"

#include "ComponentOwner.h"
#include "HealthComponent.h"

using namespace components::core;
using namespace ::testing;

class CollectableItemComponentTest : public Test
{
public:
    const utils::Vector2f position{20, 20};
    const std::string itemName{"name"};
    ComponentOwner owner{position, "CollectableItemComponentTest1"};
    ComponentOwner collector{position, "CollectableItemComponentTest2"};
    std::shared_ptr<StrictMock<ItemEffectMock>> itemEffect = std::make_shared<StrictMock<ItemEffectMock>>();
    CollectableItemComponent collectableItem{&owner, itemName, itemEffect};
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

TEST_F(CollectableItemComponentTest, use_shouldAffectCollectorAndRemoveItself)
{
    collectableItem.collectBy(&collector);
    EXPECT_CALL(*itemEffect, affect(&collector));

    collectableItem.use();

    ASSERT_TRUE(owner.shouldBeRemoved());
}

TEST_F(CollectableItemComponentTest, shouldReturnName)
{
    ASSERT_EQ(collectableItem.getName(), itemName);
}
