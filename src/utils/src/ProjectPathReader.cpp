#include "ProjectPathReader.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <limits.h>
#include <unistd.h>
#endif

#include <filesystem>
#include <iostream>
#include <optional>

#include "StringHelper.h"
#include "exceptions/DirectoryNotFound.h"

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

    const auto executablePath = getExecutablePath();
    std::filesystem::path projectRootPath;
    std::size_t projectNamePosition = 0;
    auto findNameOffset = 0;
    while (not std::filesystem::is_directory(projectRootPath) && projectNamePosition != std::string::npos)
    {
        projectNamePosition = executablePath.rfind(projectName, executablePath.size() - 1 - findNameOffset);
        projectRootPath =
            utils::StringHelper::substring(executablePath, 0, projectNamePosition + projectName.length() + 1);
        ++findNameOffset;
    }

    if (projectNamePosition == std::string::npos)
    {
        throw exceptions::DirectoryNotFound{"Project directory not found in path: " + executablePath};
    }

    projectPath = projectRootPath.string();
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