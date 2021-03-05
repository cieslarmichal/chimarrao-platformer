#pragma once

#include <stdexcept>

namespace graphics::exceptions
{
struct VisibilityLayerNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}