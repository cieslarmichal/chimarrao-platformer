#include "ProjectPathReader.h"

#include <iostream>

#include "GetExecutablePath.h"
#include "StringHelper.h"
#include "exceptions/FileNotFound.h"

namespace utils
{

std::string ProjectPathReader::getProjectRootPath()
{
    static const std::string projectName{"chimarrao-platformer"};
    static std::optional<std::string> projectPath = std::nullopt;

    if (projectPath)
    {
        return *projectPath;
    }

    const std::string currentPath = getExecutablePath();
    const auto projectNamePosition = currentPath.find(projectName);
    if (projectNamePosition == std::string::npos)
    {
        throw exceptions::FileNotFound{"Project directory not found in path: " + currentPath};
    }

    projectPath =
        utils::StringHelper::substring(currentPath, 0, projectNamePosition + projectName.length() + 1);
    return *projectPath;
}
}