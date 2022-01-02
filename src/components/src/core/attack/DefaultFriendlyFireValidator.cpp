#include "DefaultFriendlyFireValidator.h"

namespace components::core
{

DefaultFriendlyFireValidator::DefaultFriendlyFireValidator()
    : attacksWhiteList{
          {ComponentOwnerType::Player, {ComponentOwnerType::Default, ComponentOwnerType::Enemy}},
          {ComponentOwnerType::Friend, {ComponentOwnerType::Default, ComponentOwnerType::Enemy}},
          {ComponentOwnerType::Enemy,
           {ComponentOwnerType::Default, ComponentOwnerType::Player, ComponentOwnerType::Friend}}}
{
}

FriendlyFireValidationResult DefaultFriendlyFireValidator::validate(ComponentOwner* attacker,
                                                                    ComponentOwner* victim) const
{
    if (not attacker or not victim)
    {
        return FriendlyFireValidationResult::AttackNotAllowed;
    }

    if (attacksWhiteList.at(attacker->type).contains(victim->type))
    {
        return FriendlyFireValidationResult::AttackAllowed;
    }

    return FriendlyFireValidationResult::AttackNotAllowed;
}

}