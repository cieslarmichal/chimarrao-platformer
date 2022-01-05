#pragma once

#include <stdexcept>

namespace game::exceptions
{
struct InvalidDialogueTrack : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
