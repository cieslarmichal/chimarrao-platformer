#pragma once

#include <optional>

#include "CollisionLayer.h"
#include "MovementComponent.h"
#include "Rect.h"
#include "VelocityComponent.h"

namespace components::core
{
enum class Direction
{
    Left,
    Right
};
class BoxColliderComponent : public Component
{
public:
    BoxColliderComponent(ComponentOwner*, const utils::Vector2f& size,
                         CollisionLayer collisionLayer = CollisionLayer::Default,
                         const utils::Vector2f& offset = {0, 0},
                         std::shared_ptr<MovementComponent> = nullptr);

    void update(utils::DeltaTime, const input::Input&) override;
    void loadDependentComponents() override;
    bool intersects(const utils::Vector2f&);
    bool intersectsX(const std::shared_ptr<BoxColliderComponent>&);
    bool intersectsY(const std::shared_ptr<BoxColliderComponent>&);
    void resolveOverlapX(const std::shared_ptr<BoxColliderComponent>&);
    void resolveOverlapY(const std::shared_ptr<BoxColliderComponent>&);
    void setAvailableMovementDirections();
    const utils::FloatRect& getCollisionBox();
    const utils::FloatRect& getNextFrameXCollisionBox();
    const utils::FloatRect& getNextFrameYCollisionBox();
    utils::Vector2f getSize() const;
    utils::Vector2f getPosition() const;
    CollisionLayer getCollisionLayer() const;
    void setCollisionLayer(CollisionLayer layer);
    ComponentOwner* getCurrentColliderOnXAxis() const;
    void setColliderOnXAxis(ComponentOwner*);

private:
    CollisionLayer collisionLayer;
    const sf::Vector2f offset;
    utils::FloatRect collisionBoundaries;
    utils::FloatRect nextFrameCollisionBoundaries;
    std::shared_ptr<MovementComponent> movementComponent;
    std::shared_ptr<VelocityComponent> velocityComponent;
    utils::Vector2f size;
    utils::DeltaTime currentDeltaTime;
    std::unordered_map<std::string, Direction> colliderNamesWithDistancesOnXAxis;
    ComponentOwner* currentColliderOnXAxis;
};
}