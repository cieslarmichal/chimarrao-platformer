#include "PhysicsSfmlComponent.h"

namespace physics
{

PhysicsSfmlComponent::PhysicsSfmlComponent(const utils::Vector2f& sizeInit,
                                           const utils::Vector2f& positionInit)
    : direction{0, 0}, movementSpeed{0.0f, 0.0f}
{
    rectangle.setSize(sizeInit);
    rectangle.setFillColor(sf::Color::Red);
    rectangle.setPosition(positionInit);
}

void PhysicsSfmlComponent::update(const utils::DeltaTime&) {}

void PhysicsSfmlComponent::move(const utils::DeltaTime& dt)
{
    rectangle.move(static_cast<float>(direction.x) * movementSpeed.x * dt.count(),
                   static_cast<float>(direction.y) * movementSpeed.y * dt.count());
}

void PhysicsSfmlComponent::setPosition(const utils::Vector2f& position)
{
    rectangle.setPosition(position);
}

utils::Vector2f PhysicsSfmlComponent::getPosition() const
{
    return rectangle.getPosition();
}

utils::Vector2f PhysicsSfmlComponent::getSize() const
{
    return rectangle.getSize();
}

float PhysicsSfmlComponent::getLeftEdge() const
{
    return rectangle.getPosition().x;
}

float PhysicsSfmlComponent::getRightEdge() const
{
    return rectangle.getPosition().x + rectangle.getSize().x;
}

float PhysicsSfmlComponent::getBottomEdge() const
{
    return rectangle.getPosition().y + rectangle.getSize().y;
}

float PhysicsSfmlComponent::getUpperEdge() const
{
    return rectangle.getPosition().y;
}

void PhysicsSfmlComponent::setDirection(const utils::Vector2i& directionInput)
{
    direction = directionInput;
}

const Rectangle& PhysicsSfmlComponent::getRectangle() const
{
    return rectangle;
}

}