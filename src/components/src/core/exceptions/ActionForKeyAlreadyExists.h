#pragma once

#include <stdexcept>

namespace components::core::exceptions
{
struct ActionForKeyAlreadyExists : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
