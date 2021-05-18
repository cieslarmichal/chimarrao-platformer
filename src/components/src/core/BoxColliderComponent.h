#pragma once

#include "CollisionLayer.h"
#include "KeyboardMovementComponent.h"
#include "Rect.h"
#include "VelocityComponent.h"

namespace components::core
{
enum class CollisionSource
{
    Left,
    Right,
    Above,
    Below,
    None
};

class BoxColliderComponent : public Component
{
public:
    BoxColliderComponent(ComponentOwner*, const utils::Vector2f& size,
                         CollisionLayer collisionLayer = CollisionLayer::Default,
                         const utils::Vector2f& offset = {0, 0});

    void update(utils::DeltaTime, const input::Input&) override;
    void loadDependentComponents() override;
    bool intersects(const utils::Vector2f&);
    bool intersects(const std::shared_ptr<BoxColliderComponent>&);
    void resolveOverlap(const std::shared_ptr<BoxColliderComponent>&);
    void setAvailableMovementDirections();
    const utils::FloatRect& getCollisionBox();
    const utils::FloatRect& getNextFrameCollisionBox();
    CollisionLayer getCollisionLayer() const;
    void setCollisionLayer(CollisionLayer layer);

private:
    CollisionSource calculateCollisionSource(const utils::FloatRect&);

    CollisionLayer collisionLayer;
    const sf::Vector2f offset;
    utils::FloatRect collisionBoundaries;
    utils::FloatRect nextFrameCollisionBoundaries;
    std::shared_ptr<KeyboardMovementComponent> movementComponent;
    std::shared_ptr<VelocityComponent> velocityComponent;
    utils::Vector2f size;
    utils::DeltaTime currentDeltaTime;
};
}