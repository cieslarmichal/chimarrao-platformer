#pragma once

#include "ComponentOwner.h"

namespace components::core
{
class ItemEffect
{
public:
    virtual ~ItemEffect() = default;

    virtual void affect(ComponentOwner*) const = 0;
};
}