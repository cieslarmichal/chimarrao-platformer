#pragma once

#include <stdexcept>

namespace animations::exceptions
{
struct AnimationTypeNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}