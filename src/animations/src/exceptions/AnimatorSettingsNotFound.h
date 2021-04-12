#pragma once

#include <stdexcept>

namespace animations::exceptions
{
struct AnimatorSettingsNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}