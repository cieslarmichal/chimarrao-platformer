#pragma once

#include <stdexcept>

namespace game::exceptions
{
struct RequiredFieldNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
