#pragma once

#include <stdexcept>

namespace components::ui::exceptions
{
struct UIComponentConfigNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
