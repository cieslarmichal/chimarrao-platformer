#pragma once

#include "Component.h"
#include "Vector.h"
#include "VelocityComponent.h"

namespace components::core
{
class MovementComponent : public Component
{
public:
    MovementComponent(ComponentOwner*);

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
    void lock();
    void unlock();
    bool isLocked() const;

protected:
    bool canMoveRight{true};
    bool canMoveLeft{true};
    bool canMoveUp{true};
    bool canMoveDown{true};
    bool locked{false};
};
}