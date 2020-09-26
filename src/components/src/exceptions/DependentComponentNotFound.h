#pragma once

#include <stdexcept>

namespace components::exceptions
{
struct DependentComponentNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
