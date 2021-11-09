#include "DefaultComponentOwnersManager.h"

#include "CollisionSystemMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"

using namespace game;
using namespace components::core;
using namespace ::testing;

class DefaultComponentOwnersManagerTest : public Test
{
public:
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
    const utils::Vector2f position1{20, 20};
    std::shared_ptr<NiceMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<NiceMock<graphics::RendererPoolMock>>();
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    std::shared_ptr<ComponentOwner> componentOwner =
        std::make_shared<ComponentOwner>(position1, "componentOwnersMangerTest1", sharedContext);

    std::unique_ptr<StrictMock<physics::CollisionSystemMock>> collisionSystemInit{
        std::make_unique<StrictMock<physics::CollisionSystemMock>>()};
    StrictMock<physics::CollisionSystemMock>* collisionSystem{collisionSystemInit.get()};

    DefaultComponentOwnersManager componentOwnersManager{std::move(collisionSystemInit)};
};

TEST_F(DefaultComponentOwnersManagerTest, addComponentOwner_shouldNotThrow)
{
    ASSERT_NO_THROW(componentOwnersManager.add(componentOwner));
}

TEST_F(DefaultComponentOwnersManagerTest, update_shouldUpdateCollisions)
{
    EXPECT_CALL(*collisionSystem, update());

    componentOwnersManager.update(deltaTime, input);
}

TEST_F(DefaultComponentOwnersManagerTest,
       processNewObjectsWithoutAnyNewObjects_shouldNotAddOwnersInCollisionSystem)
{
    componentOwnersManager.processNewObjects();
}

TEST_F(DefaultComponentOwnersManagerTest,
       processNewObjectsWithAnyNewObjects_shouldNotAddOwnersInCollisionSystem)
{
    componentOwnersManager.add(componentOwner);
    std::vector<std::shared_ptr<components::core::ComponentOwner>> newComponentOwners{componentOwner};
    EXPECT_CALL(*collisionSystem, add(newComponentOwners));

    componentOwnersManager.processNewObjects();
}

TEST_F(DefaultComponentOwnersManagerTest, processRemovals_shouldProcessRemovalsInCollisionSystem)
{
    EXPECT_CALL(*collisionSystem, processRemovals());

    componentOwnersManager.processRemovals();
}

TEST_F(DefaultComponentOwnersManagerTest, activate_shouldNotThrow)
{
    componentOwnersManager.add(componentOwner);

    ASSERT_NO_THROW(componentOwnersManager.activate());
}

TEST_F(DefaultComponentOwnersManagerTest, deactivate_shouldNotThrow)
{
    componentOwnersManager.add(componentOwner);

    ASSERT_NO_THROW(componentOwnersManager.deactivate());
}