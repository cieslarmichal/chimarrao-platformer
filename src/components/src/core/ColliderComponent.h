#pragma once

#include "CollisionInfo.h"
#include "CollisionLayer.h"
#include "Component.h"

namespace components::core
{
class ColliderComponent : public Component
{
public:
    ColliderComponent(ComponentOwner*, CollisionLayer collisionLayer = CollisionLayer::Default);

    virtual bool intersects(const utils::Vector2f&) = 0;
    virtual CollisionInfo intersects(std::shared_ptr<ColliderComponent> other) = 0;
    virtual void resolveOverlap(const CollisionInfo&) = 0;

    CollisionLayer getCollisionLayer() const;
    void setCollisionLayer(CollisionLayer layer);

private:
    CollisionLayer collisionLayer;
};
}