#pragma once

#include <string>
#include <tuple>

#include "ItemType.h"

namespace components::core
{
struct ItemInfo
{
    const std::string name;
    const ItemType type;
};

inline bool operator==(const ItemInfo& lhs, const ItemInfo& rhs)
{
    const auto tieStruct = [](const ItemInfo& itemInfo) { return std::tie(itemInfo.name, itemInfo.type); };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::string toString(const ItemInfo& itemInfo)
{
    return "name: " + itemInfo.name + " type: " + itemInfo.type;
}

inline std::ostream& operator<<(std::ostream& os, const ItemInfo& itemInfo)
{
    return os << toString(itemInfo);
}
}