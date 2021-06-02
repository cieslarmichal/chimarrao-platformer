#include "ProjectPathReader.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <limits.h>
#include <unistd.h>
#endif

#include <optional>

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
    const auto projectNamePosition = currentPath.rfind(projectName);
    if (projectNamePosition == std::string::npos)
    {
        throw exceptions::FileNotFound{"Project directory not found in path: " + currentPath};
    }

    projectPath =
        utils::StringHelper::substring(currentPath, 0, projectNamePosition + projectName.length() + 1);
    return *projectPath;
}

std::string ProjectPathReader::getExecutablePath()
{
#ifdef _WIN32
    char path[MAX_PATH] = {0};
    GetModuleFileName(nullptr, path, MAX_PATH);
    return std::string(path);
#else
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    return std::string(result, (count > 0) ? static_cast<unsigned long>(count) : 0);
#endif
}
}