#pragma once

#include <chrono>

namespace utils
{
using DeltaTime = std::chrono::duration<float>;
using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
}