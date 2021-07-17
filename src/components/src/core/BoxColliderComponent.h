#pragma once

#include <optional>

#include "CollisionLayer.h"
#include "MovementComponent.h"
#include "Rect.h"
#include "VelocityComponent.h"

namespace components::core
{
class BoxColliderComponent : public Component
{
public:
    BoxColliderComponent(ComponentOwner*, const utils::Vector2f& size,
                         CollisionLayer collisionLayer = CollisionLayer::Default,
                         const utils::Vector2f& offset = {0, 0});

    void update(utils::DeltaTime, const input::Input&) override;
    void loadDependentComponents() override;
    bool intersects(const utils::Vector2f&);
    std::optional<utils::FloatRect> intersectsX(const std::shared_ptr<BoxColliderComponent>&);
    std::optional<utils::FloatRect> intersectsY(const std::shared_ptr<BoxColliderComponent>&);
    void resolveOverlapX(const utils::FloatRect&);
    void resolveOverlapY(const utils::FloatRect&);
    void setAvailableMovementDirections();
    const utils::FloatRect& getCollisionBox();
    const utils::FloatRect& getNextFrameXCollisionBox();
    const utils::FloatRect& getNextFrameYCollisionBox();
    CollisionLayer getCollisionLayer() const;
    void setCollisionLayer(CollisionLayer layer);

private:
    CollisionLayer collisionLayer;
    const sf::Vector2f offset;
    utils::FloatRect collisionBoundaries;
    utils::FloatRect nextFrameCollisionBoundaries;
    std::shared_ptr<MovementComponent> movementComponent;
    std::shared_ptr<VelocityComponent> velocityComponent;
    utils::Vector2f size;
    utils::DeltaTime currentDeltaTime;
};
}