#include "ColliderComponent.h"

namespace components::core
{

ColliderComponent::ColliderComponent(ComponentOwner* owner)
    : Component{owner}, collisionLayer{CollisionLayer::Default}
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