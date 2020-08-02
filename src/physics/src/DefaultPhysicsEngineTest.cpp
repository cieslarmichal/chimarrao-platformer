#include "DefaultPhysicsEngine.h"

#include <gtest/gtest.h>

using namespace ::testing;
using namespace physics;

namespace
{
const utils::Vector2f size1{20, 30};
const utils::Vector2f size2{100, 100};
const utils::Vector2f position{0, 10};
}

class DefaultPhysicsEngineTest : public Test
{
public:
    DefaultPhysicsEngine physicsEngine;
};

//void update(const utils::DeltaTime&) override;
//PhysicsId acquire(const utils::Vector2f& size, const utils::Vector2f& position) override;
//void release(const PhysicsId&) override;
//utils::Vector2f getPosition(const PhysicsId&) override;
//utils::Vector2i getMovementDirection(const PhysicsId&) const override;
//utils::Vector2f getMovementSpeed(const PhysicsId&) const override;
//void setPosition(const PhysicsId&, const utils::Vector2f&) override;
//void setMovementDirection(const PhysicsId&, const utils::Vector2i& direction) override;
//void setMovementSpeed(const PhysicsId&, const utils::Vector2f& movementSpeed) override;

TEST_F(DefaultPhysicsEngineTest, xxx)
{
    
}