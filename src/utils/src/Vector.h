#pragma once

#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>

namespace utils
{
using Vector2f = sf::Vector2f;
using Vector2i = sf::Vector2i;
using Vector2u = sf::Vector2u;
}

namespace sf
{
inline std::string toString(const utils::Vector2f& vec)
{
    return "{" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + "}";
}

inline std::string toString(const utils::Vector2i& vec)
{
    return "{" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + "}";
}

inline std::string toString(const utils::Vector2u& vec)
{
    return "{" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + "}";
}

inline std::ostream& operator<<(std::ostream& os, const utils::Vector2f& vec)
{
    return os << toString(vec);
}

inline std::ostream& operator<<(std::ostream& os, const utils::Vector2i& vec)
{
    return os << toString(vec);
}

inline std::ostream& operator<<(std::ostream& os, const utils::Vector2u& vec)
{
    return os << toString(vec);
}
}