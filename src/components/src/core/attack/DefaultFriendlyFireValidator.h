#pragma once

#include "FriendlyFireValidator.h"

namespace components::core
{
class DefaultFriendlyFireValidator : public FriendlyFireValidator
{
public:
    DefaultFriendlyFireValidator();

    FriendlyFireValidationResult validate(ComponentOwner* attacker, ComponentOwner* victim) const override;

private:
    std::unordered_map<ComponentOwnerType, std::unordered_set<ComponentOwnerType>> attacksWhiteList;
};
}
