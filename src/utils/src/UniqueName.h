#pragma once

#include <string>
#include <unordered_set>

namespace utils
{
class UniqueName
{
public:
    explicit UniqueName(std::string);
    explicit UniqueName();
    ~UniqueName();

    UniqueName(const UniqueName&) = delete;
    UniqueName& operator=(const UniqueName&) = delete;

    std::string getName() const;

private:
    std::string getIdFromGenerator();

    std::string name;
    static std::unordered_set<std::string> uniqueNames;
};
}