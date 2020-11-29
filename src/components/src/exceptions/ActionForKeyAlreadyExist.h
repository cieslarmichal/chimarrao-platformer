#pragma once

#include <stdexcept>

namespace components::exceptions
{
struct ActionForKeyAlreadyExist : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
