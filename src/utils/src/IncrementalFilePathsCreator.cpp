#include "IncrementalFilePathsCreator.h"

#include <boost/algorithm/string.hpp>
#include <sstream>

#include "StringHelper.h"

namespace utils
{
namespace
{
const auto fileNameSeparator{'/'};
}

FilePaths IncrementalFilePathsCreator::createFilePaths(const FilePath& fullPathToFirstFile,
                                                       unsigned int numberOfIncrementalFiles)
{
    if (fullPathToFirstFile.empty() || numberOfIncrementalFiles == 0)
    {
        return {};
    }

    const auto directory = extractDirectory(fullPathToFirstFile);
    const auto fileName = extractFileName(fullPathToFirstFile);

    if (not directory || not fileName)
    {
        return {};
    }

    if (auto numberFromFileName = extractNumberFromFileName(*fileName))
    {
        FilePaths filePaths;
        for (unsigned int i = 0; i < numberOfIncrementalFiles; i++)
        {
            auto currentFileNameNumber = *numberFromFileName + i;
            auto currentFileName = *fileName;
            boost::replace_all(currentFileName, std::to_string(*numberFromFileName),
                               std::to_string(currentFileNameNumber));
            filePaths.push_back(*directory + currentFileName);
        }
        return filePaths;
    }
    return {};
}

boost::optional<std::string> IncrementalFilePathsCreator::extractDirectory(const FilePath& filePath)
{
    const auto fileNameSeparatorIndex = filePath.rfind(fileNameSeparator);
    if (fileNameSeparatorIndex != std::string::npos)
    {
        return utils::StringHelper::substring(filePath, 0, fileNameSeparatorIndex + 1);
    }
    return boost::none;
}

boost::optional<std::string> IncrementalFilePathsCreator::extractFileName(const FilePath& filePath)
{
    const auto fileNameSeparatorIndex = filePath.rfind(fileNameSeparator);
    if (fileNameSeparatorIndex != std::string::npos && fileNameSeparatorIndex + 1 < filePath.size())
    {
        return utils::StringHelper::substring(filePath, fileNameSeparatorIndex + 1, filePath.size());
    }
    return boost::none;
}

boost::optional<int> IncrementalFilePathsCreator::extractNumberFromFileName(const FilePath& filePath)
{
    int numberCounter{0};
    std::string numberAsString;
    int number;

    int i = 0;
    while (i < filePath.size() && numberCounter < 2)
    {
        if (isdigit(filePath[i]))
        {
            numberCounter++;
            if (numberCounter == 1)
            {
                std::stringstream tempStream{std::string(filePath.cbegin() + i, filePath.cend())};
                tempStream >> number;
                numberAsString = std::to_string(number);
                i += static_cast<int>(numberAsString.size());
                continue;
            }
        }
        i++;
    }

    int numberPosition = static_cast<int>(filePath.find(numberAsString));
    if (numberCounter != 1 || numberPosition == std::string::npos)
    {
        return boost::none;
    }

    return std::stoi(numberAsString);
}

}