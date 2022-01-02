#pragma once

#include "ComponentOwner.h"
#include "FriendlyFireValidationResult.h"

namespace components::core
{
class FriendlyFireValidator
{
public:
    virtual ~FriendlyFireValidator() = default;

    virtual FriendlyFireValidationResult validate(ComponentOwner* attacker, ComponentOwner* victim) const = 0;
};
}