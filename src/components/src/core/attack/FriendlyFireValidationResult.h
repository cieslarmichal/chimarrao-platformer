#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

#include "StringHelper.h"

namespace components::core
{
enum class FriendlyFireValidationResult
{
    AttackAllowed,
    AttackNotAllowed
};

inline std::string toString(FriendlyFireValidationResult friendlyFireValidationResult)
{
    std::unordered_map<FriendlyFireValidationResult, std::string> friendlyFireValidationResultToString{
        {FriendlyFireValidationResult::AttackAllowed, "AttackAllowed"},
        {FriendlyFireValidationResult::AttackNotAllowed, "AttackNotAllowed"}};

    return friendlyFireValidationResultToString.at(friendlyFireValidationResult);
}

inline FriendlyFireValidationResult toCollisionLayer(const std::string& collisionLayerAsString)
{
    const auto collisionLayerAsLowerString = utils::StringHelper::getLowerCases(collisionLayerAsString);

    std::unordered_map<std::string, FriendlyFireValidationResult> stringToCollisionLayer{
        {"attackAllowed", FriendlyFireValidationResult::AttackNotAllowed},
        {"attackNotAllowed", FriendlyFireValidationResult::AttackNotAllowed}};

    return stringToCollisionLayer.at(collisionLayerAsLowerString);
}

inline std::ostream& operator<<(std::ostream& os, FriendlyFireValidationResult collisionLayer)
{
    return os << "FriendlyFireValidationResult::" << toString(collisionLayer);
}
}