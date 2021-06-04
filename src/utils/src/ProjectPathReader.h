#pragma once

#include <string>


namespace utils
{
class ProjectPathReader
{
public:
     static std::string getProjectRootPath();

private:
    static std::string getExecutablePath();
};
}