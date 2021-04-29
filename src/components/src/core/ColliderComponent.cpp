#include "ColliderComponent.h"

namespace components::core
{

ColliderComponent::ColliderComponent(ComponentOwner* owner, CollisionLayer collisionLayerInit)
    : Component{owner}, collisionLayer{collisionLayerInit}
{
}

CollisionLayer ColliderComponent::getCollisionLayer() const
{
    return collisionLayer;
}

void ColliderComponent::setCollisionLayer(CollisionLayer layer)
{
    collisionLayer = layer;
}
}