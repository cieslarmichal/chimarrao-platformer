#pragma once

#include <string>
#include <optional>

#include "UtilsApi.h"

namespace utils
{
class ProjectPathReader
{
public:
    UTILS_API static std::string getProjectRootPath();
};
}