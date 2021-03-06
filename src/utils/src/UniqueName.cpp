#include "UniqueName.h"

#include "exceptions/NameIsNotUnique.h"

namespace utils
{
std::unordered_set<std::string> UniqueName::uniqueNames{};

UniqueName::UniqueName(std::string nameInit) : name{std::move(nameInit)}
{
    if (uniqueNames.count(name))
    {
        throw exceptions::NameIsNotUnique{name + " is already used in program"};
    }

    uniqueNames.insert(name);
}

std::string UniqueName::getName() const
{
    return name;
}

}
