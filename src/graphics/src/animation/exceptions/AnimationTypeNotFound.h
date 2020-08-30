#pragma once

#include <stdexcept>

namespace graphics::animation::exceptions
{
struct AnimationTypeNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}