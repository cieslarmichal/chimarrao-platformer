#pragma once

#include "SFML/Graphics/Rect.hpp"

#include "Component.h"
#include "Vector.h"

namespace components::core
{
class HitBoxComponent : public Component
{
public:
    HitBoxComponent(ComponentOwner*, const utils::Vector2f& size, const utils::Vector2f& offset = {0, 0});

    void lateUpdate(utils::DeltaTime) override;
    bool intersects(const utils::Vector2f& position) const;

private:
    utils::Vector2f originPosition;
    const utils::Vector2f size;
    const utils::Vector2f offset;
};
}