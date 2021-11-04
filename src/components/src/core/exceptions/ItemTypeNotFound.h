#pragma once

#include <stdexcept>

namespace components::core::exceptions
{
struct ItemTypeNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
