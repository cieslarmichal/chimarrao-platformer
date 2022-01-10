#include "DistanceAttack.h"

#include "gtest/gtest.h"

#include "AnimatorMock.h"
#include "ComponentOwnersManagerMock.h"
#include "RendererPoolMock.h"

#include "DirectionComponent.h"
#include "exceptions/DependentComponentNotFound.h"

using namespace ::testing;
using namespace components::core;

class DistanceAttackTest : public Test
{
public:
    DistanceAttackTest()
    {
        componentOwner1.addComponent<VelocityComponent>(6);
        componentOwner1.addComponent<DirectionComponent>();
        componentOwner1.addComponent<AnimationComponent>(animator);
        componentOwner1.addComponent<BoxColliderComponent>(size);
        componentOwner1.loadDependentComponents();
    }

    const utils::Vector2f size{5, 5};
    const utils::Vector2f position1{20, 20};
    std::shared_ptr<StrictMock<animations::AnimatorMock>> animator =
        std::make_shared<StrictMock<animations::AnimatorMock>>();
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    ComponentOwner componentOwner1{position1, "DistanceAttackTest1", sharedContext};
    std::shared_ptr<StrictMock<components::core::ComponentOwnersManagerMock>> componentOwnersManager{
        std::make_shared<StrictMock<components::core::ComponentOwnersManagerMock>>()};
    DistanceAttack distanceAttack{&componentOwner1, sharedContext, componentOwnersManager};
};

TEST_F(DistanceAttackTest,
       givenComponentOwnerWithoutDirectionComponent_attack_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutDirection{position1, "componentOwnerWithoutDirection", sharedContext};
    DistanceAttack distanceAttackWithoutDirection{&componentOwnerWithoutDirection, sharedContext, componentOwnersManager};

    ASSERT_THROW(distanceAttackWithoutDirection.attack(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(DistanceAttackTest,
       givenComponentOwnerWithoutBoxColliderComponent_attack_shouldThrowDependentComponentNotFound)
{
    ComponentOwner componentOwnerWithoutBoxCollider{position1, "componentOwnerWithoutBoxCollider",
                                                    sharedContext};
    componentOwnerWithoutBoxCollider.addComponent<VelocityComponent>(6);
    componentOwnerWithoutBoxCollider.addComponent<DirectionComponent>();
    DistanceAttack distanceAttackWithoutBoxCollider{&componentOwnerWithoutBoxCollider, sharedContext, componentOwnersManager};

    ASSERT_THROW(distanceAttackWithoutBoxCollider.attack(),
                 components::core::exceptions::DependentComponentNotFound);
}

TEST_F(DistanceAttackTest, attack_shouldAddNewObjectToComponentOwnersManager)
{
    EXPECT_CALL(*animator, getAnimationDirection()).WillOnce(Return(animations::AnimationDirection::Left));
    EXPECT_CALL(*componentOwnersManager, add(_));

    distanceAttack.attack();
}
