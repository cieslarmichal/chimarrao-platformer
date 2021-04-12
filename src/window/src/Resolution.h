#pragma once

#include <ostream>
#include <string>
#include <tuple>

namespace window
{
struct Resolution
{
    unsigned int width;
    unsigned int height;
};

inline bool operator==(const Resolution& lhs, const Resolution& rhs)
{
    auto tieStruct = [](const Resolution& resolution)
    { return std::tie(resolution.width, resolution.height); };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline bool operator!=(const Resolution& lhs, const Resolution& rhs)
{
    return !(lhs == rhs);
}

inline std::string toString(const Resolution& resolution)
{
    return std::to_string(resolution.width) + "x" + std::to_string(resolution.height);
}

inline std::ostream& operator<<(std::ostream& os, const Resolution& resolution)
{
    return os << toString(resolution);
}
}