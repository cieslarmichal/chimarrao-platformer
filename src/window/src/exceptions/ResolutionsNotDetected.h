#pragma once

#include <stdexcept>

namespace window::exceptions
{
struct ResolutionsNotDetected : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}