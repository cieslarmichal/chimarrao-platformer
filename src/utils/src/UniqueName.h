#pragma once

#include <string>
#include <unordered_set>

#include "UtilsApi.h"

namespace utils
{
class UniqueName
{
public:
    UTILS_API UniqueName(std::string);
    UTILS_API ~UniqueName();

    UniqueName(const UniqueName&) = delete;
    UniqueName& operator=(const UniqueName&) = delete;

    UTILS_API std::string getName() const;

private:
    std::string name;
    static std::unordered_set<std::string> uniqueNames;
};
}