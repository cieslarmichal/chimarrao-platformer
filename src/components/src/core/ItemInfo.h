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
    const graphics::TexturePath texturePath;
};

inline bool operator==(const ItemInfo& lhs, const ItemInfo& rhs)
{
    const auto tieStruct = [](const ItemInfo& itemInfo)
    { return std::tie(itemInfo.name, itemInfo.type, itemInfo.texturePath); };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::string toString(const ItemInfo& itemInfo)
{
    return "name: " + itemInfo.name + " type: " + toString(itemInfo.type) +
           " texture path: " + itemInfo.texturePath;
}

inline std::ostream& operator<<(std::ostream& os, const ItemInfo& itemInfo)
{
    return os << toString(itemInfo);
}
}