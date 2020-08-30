#pragma once

#include <stdexcept>

namespace graphics::exceptions
{
struct CannotAccessTextureFile : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
