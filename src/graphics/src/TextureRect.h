#pragma once

#include <optional>
#include <string>

#include "IntRect.h"

namespace graphics
{
struct TextureRect
{
    std::string texturePath;
    std::optional<utils::IntRect> rectToCutTexture;
};

inline bool operator==(const TextureRect& lhs, const TextureRect& rhs)
{
    const auto tieStruct = [](const TextureRect& textureRect) {
        return std::tie(textureRect.texturePath, textureRect.rectToCutTexture);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::string toString(const TextureRect& textureRect)
{
    const auto rectAsString = textureRect.rectToCutTexture ? toString(*textureRect.rectToCutTexture) : "none";
    return "texturePath: " + textureRect.texturePath + " rect: " + rectAsString;
}

inline std::ostream& operator<<(std::ostream& os, const TextureRect& textureRect)
{
    return os << toString(textureRect);
}

struct TextureRectHash
{
    std::size_t operator()(const TextureRect& textureRect) const
    {
        return std::hash<std::string>()(toString(textureRect));
    };
};
}