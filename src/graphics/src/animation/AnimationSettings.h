#pragma once

#include <ostream>
#include <string>
#include <tuple>

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

inline std::ostream& operator<<(std::ostream& os, const AnimationSettings& animationSettings)
{
    return os << "animationType: " << animationSettings.animationType
              << " firstTexturePath: " << animationSettings.firstTexturePath
              << " numberOfTextures: " << animationSettings.numberOfTextures
              << " timeBetweenTexturesInSeconds: " << animationSettings.timeBetweenTexturesInSeconds;
}

using AnimationsSettings = std::vector<AnimationSettings>;
}