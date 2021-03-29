#pragma once

#include <stdexcept>

namespace utils::exceptions
{
struct NameIsNotUnique : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
