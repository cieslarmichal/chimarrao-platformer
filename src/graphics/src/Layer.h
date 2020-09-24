#pragma once

namespace graphics {
enum class Layer {
    First,
    Second,
    Third,
    Background
};
inline void xyz()
{
    Layer::First < Layer::Second;
}
}