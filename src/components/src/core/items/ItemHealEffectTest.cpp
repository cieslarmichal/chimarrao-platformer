#include "ItemHealEffect.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "ComponentOwner.h"
#include "health/HealthComponent.h"
#include "exceptions/DependentComponentNotFound.h"

using namespace components::core;
using namespace ::testing;

class ItemHealEffectTest : public Test
{
public:
    ItemHealEffectTest()
    {
        collector.addComponent<HealthComponent>(initialHealthPoints);
        collector.loadDependentComponents();
    }

    const utils::Vector2f position{20, 20};
    const unsigned int initialHealthPoints{100};
    const unsigned int healthPointsToHeal{1};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner collector{position, "ItemHealEffectTest1", sharedContext};
    ComponentOwner collectorWithNoHealth{position, "ItemHealEffectTest2", sharedContext};
    ItemHealEffect itemHealEffect{healthPointsToHeal};
};

TEST_F(ItemHealEffectTest, givenNoHealthComponent_shouldThrow)
{
    ASSERT_THROW(itemHealEffect.affect(&collectorWithNoHealth), exceptions::DependentComponentNotFound);
}

TEST_F(ItemHealEffectTest, givenHealthComponent_shouldAddHealthPoints)
{
    const auto health = collector.getComponent<HealthComponent>();
    const auto lostHealthPoints = 5;
    health->loseHealthPoints(lostHealthPoints);

    itemHealEffect.affect(&collector);

    ASSERT_EQ(health->getCurrentHealth(), initialHealthPoints - lostHealthPoints + 1);
}
