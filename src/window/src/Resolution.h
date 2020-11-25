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
    auto tieStruct = [](const Resolution& resolution) {
        return std::tie(resolution.width, resolution.height);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::ostream& operator<<(std::ostream& os, const Resolution& resolution)
{
    return os << "width: " << resolution.width << " height: " << resolution.height;
}
}