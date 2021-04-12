#pragma once

#include <string>
#include <unordered_set>

#include "UtilsApi.h"

namespace utils
{
class UTILS_API UniqueName
{
public:
    UniqueName(std::string);
    ~UniqueName();

    UniqueName(const UniqueName&) = delete;
    UniqueName& operator=(const UniqueName&) = delete;

    std::string getName() const;

private:
    std::string name;
    static std::unordered_set<std::string> uniqueNames;
};
}