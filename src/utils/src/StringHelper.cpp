#include "StringHelper.h"

#include <algorithm>

#include "boost/algorithm/string.hpp"

namespace utils
{

std::string StringHelper::join(const std::vector<std::string>& contentWithLines, const std::string& joinWith)
{
    return boost::algorithm::join(contentWithLines, joinWith);
}

std::vector<std::string> StringHelper::split(const std::string& content, const std::string& splitBy)
{
    std::vector<std::string> contentWithLines;
    boost::split(contentWithLines, content, boost::is_any_of(splitBy));
    return contentWithLines;
}

std::string StringHelper::cutOffFromString(const std::string& line, size_t startIndexToCut,
                                           size_t endIndexToCut)
{
    const auto sizeOfLine = line.size();
    const auto head = line.substr(0, startIndexToCut);
    const auto tail = line.substr(endIndexToCut + 1, sizeOfLine - endIndexToCut + 1);
    return head + tail;
}

std::vector<std::string> StringHelper::getNonEmptyLines(const std::vector<std::string>& lines)
{
    std::vector<std::string> nonEmptyLines;
    for (const auto& line : lines)
    {
        if (not line.empty())
        {
            nonEmptyLines.push_back(line);
        }
    }
    return nonEmptyLines;
}

void StringHelper::removeEmptyLines(std::vector<std::string>& lines)
{
    lines.erase(std::remove_if(lines.begin(), lines.end(), [](std::string& line) { return line.empty(); }),
                lines.end());
}

void StringHelper::trim(std::vector<std::string>& lines)
{
    for (auto& line : lines)
    {
        trim(line);
    }
}

void StringHelper::removeDuplicates(std::vector<std::string>& lines)
{
    sort(lines.begin(), lines.end());
    lines.erase(unique(lines.begin(), lines.end()), lines.end());
}

void StringHelper::cutOffString(std::string& line, size_t startIndexToCutIncluded,
                                size_t endIndexToCutIncluded)
{
    line = cutOffFromString(line, startIndexToCutIncluded, endIndexToCutIncluded);
}

std::string StringHelper::substring(const std::string& line, size_t startIndex, size_t endIndex)
{
    return line.substr(startIndex, endIndex - startIndex);
}

std::string StringHelper::getLowerCases(const std::string& input)
{
    return boost::algorithm::to_lower_copy(input);
}

void StringHelper::trim(std::string& line)
{
    boost::algorithm::trim(line);
}

}
