#pragma once

#include <stdexcept>

namespace components::ui::exceptions
{
struct UIConfigNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
