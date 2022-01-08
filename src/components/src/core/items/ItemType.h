#pragma once

#include <iostream>
#include <unordered_map>

#include "StringHelper.h"
#include "exceptions/ItemTypeNotFound.h"

namespace components::core
{
enum class ItemType
{
    Yerba,
    Meat,
    Apple,
    Blueberries
};

inline std::string toString(ItemType itemType)
{
    std::unordered_map<ItemType, std::string> itemTypeToString{{ItemType::Yerba, "Yerba"},
                                                               {ItemType::Meat, "Meat"},
                                                               {ItemType::Apple, "Apple"},
                                                               {ItemType::Blueberries, "Blueberries"}};

    try
    {
        return itemTypeToString.at(itemType);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::ItemTypeNotFound{e.what()};
    }
}

inline ItemType toItemType(const std::string& itemTypeAsString)
{
    const auto itemTypeLowerString = utils::StringHelper::getLowerCases(itemTypeAsString);

    std::unordered_map<std::string, ItemType> stringToItemType{{"yerba", ItemType::Yerba},
                                                               {"meat", ItemType::Meat},
                                                               {"apple", ItemType::Apple},
                                                               {"blueberries", ItemType::Blueberries}};

    try
    {
        return stringToItemType.at(itemTypeLowerString);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::ItemTypeNotFound{e.what()};
    }
}

inline std::ostream& operator<<(std::ostream& os, ItemType itemType)
{
    return os << "ItemType::" << toString(itemType);
}
}