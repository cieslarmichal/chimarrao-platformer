#include "TransformComponent.h"

namespace components::core
{

TransformComponent::TransformComponent(ComponentOwner* ownerInit, const utils::Vector2f& positionInit)
    : Component{ownerInit}, position{positionInit}
{
}

void TransformComponent::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}
void TransformComponent::setPosition(const utils::Vector2f& pos)
{
    position = pos;
}

void TransformComponent::addPosition(float deltaX, float deltaY)
{
    position.x += deltaX;
    position.y += deltaY;
}

void TransformComponent::addPosition(const utils::Vector2f& deltaPosition)
{
    position += deltaPosition;
}

void TransformComponent::setX(float x)
{
    position.x = x;
}

void TransformComponent::setY(float y)
{
    position.y = y;
}

const utils::Vector2f& TransformComponent::getPosition() const
{
    return position;
}
}