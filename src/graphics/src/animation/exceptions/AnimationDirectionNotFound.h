#pragma once

#include <stdexcept>

namespace graphics::animation::exceptions
{
struct AnimationDirectionNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}