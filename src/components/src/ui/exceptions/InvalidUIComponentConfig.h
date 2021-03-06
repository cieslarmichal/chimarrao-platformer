#pragma once

#include <stdexcept>

namespace components::ui::exceptions
{
struct InvalidUIComponentConfig : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
