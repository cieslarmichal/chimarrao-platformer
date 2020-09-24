#pragma once

#include <stdexcept>

namespace animations::exceptions
{
struct TexturesNotInitialized : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}