#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

#include "StringHelper.h"

namespace components::core
{
enum class ComponentOwnerType
{
    Default,
    Player,
    Friend,
    Enemy,
};

inline std::string toString(ComponentOwnerType componentOwnerType)
{
    std::unordered_map<ComponentOwnerType, std::string> componentOwnerTypeToString{
        {ComponentOwnerType::Default, "Default"},
        {ComponentOwnerType::Player, "Player"},
        {ComponentOwnerType::Friend, "Friend"},
        {ComponentOwnerType::Enemy, "Enemy"}};

    return componentOwnerTypeToString.at(componentOwnerType);
}

inline ComponentOwnerType toComponentOwnerType(const std::string& componentOwnerTypeAsString)
{
    const auto componentOwnerTypeAsLowerString =
        utils::StringHelper::getLowerCases(componentOwnerTypeAsString);

    std::unordered_map<std::string, ComponentOwnerType> stringToComponentOwnerType{
        {"default", ComponentOwnerType::Default},
        {"player", ComponentOwnerType::Player},
        {"friend", ComponentOwnerType::Friend},
        {"enemy", ComponentOwnerType::Enemy}};

    return stringToComponentOwnerType.at(componentOwnerTypeAsLowerString);
}

inline std::ostream& operator<<(std::ostream& os, ComponentOwnerType componentOwnerType)
{
    return os << "ComponentOwnerType::" << toString(componentOwnerType);
}
}