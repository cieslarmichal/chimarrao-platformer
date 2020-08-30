#pragma once

#include <stdexcept>

namespace graphics::exceptions
{
struct CannotAccess : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}

