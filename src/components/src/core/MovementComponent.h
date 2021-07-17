#pragma once

#include "Component.h"
#include "Vector.h"
#include "VelocityComponent.h"

namespace components::core
{
class MovementComponent : public Component
{
public:
    MovementComponent(ComponentOwner*, float movementSpeed);
    virtual ~MovementComponent() = default;

    void setMovementSpeed(float speed);
    float getMovementSpeed() const;
    void allowMoveRight();
    void blockMoveRight();
    void allowMoveLeft();
    void blockMoveLeft();
    void allowMoveUp();
    void blockMoveUp();
    void allowMoveDown();
    void blockMoveDown();
    bool isAllowedToMoveRight() const;
    bool isAllowedToMoveLeft() const;
    bool isAllowedToMoveUp() const;
    bool isAllowedToMoveDown() const;

protected:
    float movementSpeed;
    bool canMoveRight{true};
    bool canMoveLeft{true};
    bool canMoveUp{true};
    bool canMoveDown{true};
};
}