#pragma once

#include "DeltaTime.h"

namespace game
{
class Entity
{
public:
    virtual ~Entity() = default;

    virtual void update(const utils::DeltaTime&) = 0;
    virtual bool isDead() = 0;
};
}