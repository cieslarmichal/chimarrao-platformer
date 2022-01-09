#include "CollectableItemComponent.h"

#include "gtest/gtest.h"

#include "ItemEffectMock.h"
#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "health/HealthComponent.h"

using namespace components::core;
using namespace ::testing;

class CollectableItemComponentTest : public Test
{
public:
    const utils::Vector2f position{20, 20};
    const std::string itemName1{"name1"};
    const std::string itemName2{"name2"};
    const ItemType itemType{ItemType::Apple};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner owner{position, "CollectableItemComponentTest1", sharedContext};
    ComponentOwner collector{position, "CollectableItemComponentTest2", sharedContext};
    std::shared_ptr<StrictMock<ItemEffectMock>> itemEffect = std::make_shared<StrictMock<ItemEffectMock>>();
    CollectableItemComponent collectableItem{&owner, itemName1, itemType, itemEffect};
    CollectableItemComponent collectableItemWithoutEffect{&owner, itemName2, itemType, nullptr};
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

TEST_F(CollectableItemComponentTest, useWithoutCollector_shouldNotRemoveItself)
{
    collectableItem.use();

    ASSERT_FALSE(owner.shouldBeRemoved());
}

TEST_F(CollectableItemComponentTest, useWithoutEffect_shouldNotRemoveItself)
{
    collectableItemWithoutEffect.use();

    ASSERT_FALSE(owner.shouldBeRemoved());
}

TEST_F(CollectableItemComponentTest, shouldReturnName)
{
    ASSERT_EQ(collectableItem.getName(), itemName1);
}

TEST_F(CollectableItemComponentTest, shouldReturnItemType)
{
    ASSERT_EQ(collectableItem.getType(), itemType);
}
