#pragma once

#include <string>

#include "UtilsApi.h"

namespace utils
{
class ProjectPathReader
{
public:
    UTILS_API static std::string getProjectRootPath();

private:
    static std::string getExecutablePath();
};
}