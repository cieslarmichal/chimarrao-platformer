#pragma once

#include <string>

#include "TexturePath.h"

namespace graphics::animation
{
struct AnimationSettings
{
    std::string animationType;
    TexturePath firstTexturePath;
    int numberOfTextures;
    float timeBetweenTexturesInSeconds;
};

inline bool operator==(const AnimationSettings& lhs, const AnimationSettings& rhs)
{
    auto tieStruct = [](const AnimationSettings& settings) {
        return std::tie(settings.animationType, settings.firstTexturePath, settings.numberOfTextures,
                        settings.timeBetweenTexturesInSeconds);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}

using AnimationsSettings = std::vector<AnimationSettings>;
}