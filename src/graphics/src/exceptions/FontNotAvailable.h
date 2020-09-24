#pragma once

#include <stdexcept>

namespace graphics::exceptions
{
struct FontNotAvailable : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
