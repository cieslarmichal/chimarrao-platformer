#pragma once

#include <stdexcept>

namespace game::exceptions
{
struct StatesStatusNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
