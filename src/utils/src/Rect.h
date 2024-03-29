#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <iostream>
#include <string>

namespace utils
{
using IntRect = sf::IntRect;
using FloatRect = sf::FloatRect;
}

namespace sf
{
inline std::string toString(const utils::IntRect& rect)
{
    return "point: {" + std::to_string(rect.left) + ", " + std::to_string(rect.top) + "} size: {" +
           std::to_string(rect.width) + ", " + std::to_string(rect.height) + "}";
}

inline std::string toString(const utils::FloatRect& rect)
{
    return "point: {" + std::to_string(rect.left) + ", " + std::to_string(rect.top) + "} size: {" +
           std::to_string(rect.width) + ", " + std::to_string(rect.height) + "}";
}

inline std::ostream& operator<<(std::ostream& os, const utils::IntRect& rect)
{
    return os << toString(rect);
}

inline std::ostream& operator<<(std::ostream& os, const utils::FloatRect& rect)
{
    return os << toString(rect);
}

}