#pragma once

#include <stdexcept>

namespace utils::exceptions
{
struct PathIsNotAFile : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

}