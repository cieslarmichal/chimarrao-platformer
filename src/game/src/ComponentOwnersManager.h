#pragma once

#include <memory>
#include <vector>

#include "ComponentOwner.h"
#include "DeltaTime.h"
#include "Input.h"

namespace game
{
class ComponentOwnersManager
{
public:
    virtual ~ComponentOwnersManager() = default;

    virtual void add(std::shared_ptr<components::core::ComponentOwner>) = 0;
    virtual void update(const utils::DeltaTime&, const input::Input&) = 0;
    virtual void processNewObjects() = 0;
    virtual void processRemovals() = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
};
}
