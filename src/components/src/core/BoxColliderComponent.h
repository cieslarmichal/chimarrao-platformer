#pragma once

#include "ColliderComponent.h"
#include "Rect.h"

namespace components::core
{
enum class CollisionSource
{
    Left,
    Right,
    Above,
    Below
};

class BoxColliderComponent : public ColliderComponent
{
public:
    BoxColliderComponent(ComponentOwner*, const utils::Vector2f& size,
                         const utils::Vector2f& offset = {0, 0});

    bool intersects(const utils::Vector2f&) override;
    CollisionInfo intersects(std::shared_ptr<ColliderComponent>) override;
    void resolveOverlap(const CollisionInfo&) override;

    const utils::FloatRect& getCollisionBox();

private:
    static CollisionSource calculateCollisionSource(const utils::FloatRect&, const utils::FloatRect&);
    void setPosition();

    utils::FloatRect rect;
    sf::Vector2f offset;
};
}