#pragma once

#include <stdexcept>

namespace components::core::exceptions
{
struct InvalidCapacity : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
