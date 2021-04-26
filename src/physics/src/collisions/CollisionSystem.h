#pragma once

#include <vector>
#include <memory>

#include "core/Component.h"

class CollisionSystem
{
public:
    ~CollisionSystem() = default;

    virtual void add(std::vector<std::shared_ptr<components::core::Component>>&) = 0;
    virtual void processRemovals() = 0;
    virtual void update() = 0;
};