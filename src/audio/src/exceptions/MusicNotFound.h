#pragma once

#include <stdexcept>

namespace audio::exceptions
{
struct MusicNotFound : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
