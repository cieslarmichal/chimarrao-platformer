#pragma once

#include "gmock/gmock.h"

#include "Quadtree.h"

namespace physics
{
class QuadtreeMock : public Quadtree
{
public:
    MOCK_METHOD(void, insertCollider, (const std::shared_ptr<components::core::BoxColliderComponent>&), (override));
    MOCK_METHOD(void, removeCollider, (const std::shared_ptr<components::core::BoxColliderComponent>&), (override));
    MOCK_METHOD(void, clearAllColliders, (), ());
    MOCK_METHOD(const utils::FloatRect&, getNodeBounds, (), (const override));
    MOCK_METHOD(std::vector<std::shared_ptr<components::core::BoxColliderComponent>>,
    getCollidersIntersectingWithAreaFromX, (const utils::FloatRect& area), (const override));
    MOCK_METHOD(std::vector<std::shared_ptr<components::core::BoxColliderComponent>>,
    getCollidersIntersectingWithAreaFromY, (const utils::FloatRect& area), (const override));
};
}