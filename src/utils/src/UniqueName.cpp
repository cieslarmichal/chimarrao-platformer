#include "UniqueName.h"

#include "UniqueIdGenerator.h"
#include "exceptions/NameIsNotUnique.h"

namespace utils
{
std::unordered_set<std::string> UniqueName::uniqueNames{};

UniqueName::UniqueName(std::string nameInit) : name{std::move(nameInit)}
{
    if (uniqueNames.count(name))
    {
        throw exceptions::NameIsNotUnique{"'" + name + "' is already used in program"};
    }

    uniqueNames.insert(name);
}

UniqueName::UniqueName() : name{getIdFromGenerator()}
{
    if (uniqueNames.count(name))
    {
        throw exceptions::NameIsNotUnique{name + " is already used in program"};
    }

    uniqueNames.insert(name);
}

UniqueName::~UniqueName()
{
    uniqueNames.erase(name);
}

std::string UniqueName::getName() const
{
    return name;
}
std::string UniqueName::getIdFromGenerator()
{
    std::string uniqueId;
    for (const auto& dataEntry : UniqueIdGenerator::generateId().data)
    {
        uniqueId += std::to_string(dataEntry);
    }
    return uniqueId;
}

}
