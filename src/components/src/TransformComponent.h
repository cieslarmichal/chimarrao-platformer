#pragma once

#include "Component.h"
#include "Vector.h"

namespace components
{
class TransformComponent : public Component
{
public:
    TransformComponent(ComponentOwner*, const utils::Vector2f&);

    void setPosition(float x, float y);
    void setPosition(const utils::Vector2f&);
    void addPosition(float deltaX, float deltaY);
    void addPosition(const utils::Vector2f& deltaPosition);
    void setX(float x);
    void setY(float y);
    [[nodiscard]] const utils::Vector2f& getPosition() const;

private:
    utils::Vector2f position;
};
}
