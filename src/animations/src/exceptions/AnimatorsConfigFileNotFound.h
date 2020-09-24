#pragma once

#include <stdexcept>

namespace animations::exceptions
{
struct AnimatorsConfigFileNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}