#pragma once

#include <stdexcept>

namespace audio::exceptions
{
struct MusicNotAvailable : std::runtime_error
{
    using std::runtime_error::runtime_error;
};
}
