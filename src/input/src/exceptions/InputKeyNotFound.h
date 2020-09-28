#pragma once

#include <stdexcept>

namespace input::exceptions
{
struct InputKeyNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}