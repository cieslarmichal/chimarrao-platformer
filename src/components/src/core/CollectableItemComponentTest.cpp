#include "CollectableItemComponent.h"

#include "gtest/gtest.h"

#include "ItemEffectMock.h"
#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "HealthComponent.h"

using namespace components::core;
using namespace ::testing;

class CollectableItemComponentTest : public Test
{
public:
    const utils::Vector2f position{20, 20};
    const std::string itemName{"name"};
    const ItemType itemType{ItemType::Apple};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner owner{position, "CollectableItemComponentTest1", sharedContext};
    ComponentOwner collector{position, "CollectableItemComponentTest2", sharedContext};
    std::shared_ptr<StrictMock<ItemEffectMock>> itemEffect = std::make_shared<StrictMock<ItemEffectMock>>();
    CollectableItemComponent collectableItem{&owner, itemName, itemType, itemEffect};
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

TEST_F(CollectableItemComponentTest, shouldReturnItemType)
{
    ASSERT_EQ(collectableItem.getType(), itemType);
}
