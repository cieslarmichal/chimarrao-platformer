#pragma once

#include <iostream>

#include "SFML/System/Vector2.hpp"

namespace utils
{
using Vector2f = sf::Vector2f;
using Vector2i = sf::Vector2i;
using Vector2u = sf::Vector2u;
}

namespace sf
{
inline std::ostream& operator<<(std::ostream& os, const utils::Vector2f& vec)
{
    return os << "{" << vec.x << ", " << vec.y << "}";
}

inline std::ostream& operator<<(std::ostream& os, const utils::Vector2i& vec)
{
    return os << "{" << vec.x << ", " << vec.y << "}";
}

inline std::ostream& operator<<(std::ostream& os, const utils::Vector2u& vec)
{
    return os << "{" << vec.x << ", " << vec.y << "}";
}
}