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

inline FriendlyFireValidationResult toFriendlyFireValidationResult(const std::string& friendlyFireAsString)
{
    const auto friendlyFireAsLowerString = utils::StringHelper::getLowerCases(friendlyFireAsString);

    std::unordered_map<std::string, FriendlyFireValidationResult> stringToFriendlyFireValidationResult{
        {"attackAllowed", FriendlyFireValidationResult::AttackNotAllowed},
        {"attackNotAllowed", FriendlyFireValidationResult::AttackNotAllowed}};

    return stringToFriendlyFireValidationResult.at(friendlyFireAsLowerString);
}

inline std::ostream& operator<<(std::ostream& os, FriendlyFireValidationResult friendlyFireValidationResult)
{
    return os << "FriendlyFireValidationResult::" << toString(friendlyFireValidationResult);
}
}