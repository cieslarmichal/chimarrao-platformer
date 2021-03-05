#include "HitboxComponent.h"

#include "gtest/gtest.h"

#include "ComponentOwner.h"
#include "DeltaTime.h"

using namespace components::core;
using namespace ::testing;

class HitboxComponentTest : public Test
{
public:
    const utils::Vector2f size{5, 5};
    const utils::Vector2f offset{1, 1};
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f positionInsideTarget{21, 21};
    const utils::Vector2f positionOutsideTarget{27, 21};
    ComponentOwner componentOwner{position1};
    utils::DeltaTime deltaTime{1};
    HitboxComponent hitboxComponent{&componentOwner, size, offset};
};

TEST_F(HitboxComponentTest, givenPositionInsideTarget_shouldIntersect)
{
    hitboxComponent.lateUpdate(deltaTime);

    ASSERT_TRUE(hitboxComponent.intersects(positionInsideTarget));
}

TEST_F(HitboxComponentTest, givenPositionOutsideTarget_shouldNotIntersect)
{
    hitboxComponent.lateUpdate(deltaTime);

    ASSERT_FALSE(hitboxComponent.intersects(positionOutsideTarget));
}
