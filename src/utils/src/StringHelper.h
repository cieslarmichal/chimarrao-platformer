#pragma once

#include <string>
#include <vector>

namespace utils
{
class StringHelper
{
public:
    static std::string join(const std::vector<std::string>& contentWithLines,
                            const std::string& joinWith = "\n");
    static std::vector<std::string> split(const std::string& content, const std::string& splitBy = "\n");
    static std::vector<std::string> getNonEmptyLines(const std::vector<std::string>&);
    static void removeEmptyLines(std::vector<std::string>&);
    static void trim(std::vector<std::string>&);
    static void removeDuplicates(std::vector<std::string>&);
    static std::string cutOffFromString(const std::string& line, size_t startIndexToCut,
                                        size_t endIndexToCut);
    static void cutOffString(std::string& line, size_t startIndexToCutIncluded, size_t endIndexToCutIncluded);
    static std::string substring(const std::string& line, size_t startIndex, size_t endIndex);
    static std::string getLowerCases(const std::string&);
    static void trim(std::string&);
};

}
