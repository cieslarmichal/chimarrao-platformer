#pragma once

#include <stdexcept>

namespace graphics::animation::exceptions
{
struct AnimationTypeNotSupported : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}