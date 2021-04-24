#pragma once

#include "Component.h"
#include "Rect.h"

namespace components::core
{

enum class CollisionLayer
{
    Default,
    Player,
    Tile
};

struct CollisionInfo
{
    bool colliding = false;
    const utils::FloatRect* other;
};

class ColliderComponent : public Component
{
public:
    ColliderComponent(ComponentOwner*);

    virtual CollisionInfo intersects(std::shared_ptr<ColliderComponent> other) = 0;
    virtual void resolveOverlap(const CollisionInfo&) = 0;

    CollisionLayer getCollisionLayer() const;
    void setCollisionLayer(CollisionLayer layer);

private:
    CollisionLayer collisionLayer;
};
}