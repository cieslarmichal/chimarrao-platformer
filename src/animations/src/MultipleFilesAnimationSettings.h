#pragma once

#include <ostream>
#include <string>
#include <tuple>

#include "TexturePath.h"

namespace animations
{
struct MultipleFilesAnimationSettings
{
    std::string animationType;
    graphics::TexturePath firstTexturePath;
    int numberOfTextures;
    float timeBetweenTexturesInSeconds;
    bool loopsAllowed{true};
    bool interruptionAllowed{true};
};

inline bool operator==(const MultipleFilesAnimationSettings& lhs, const MultipleFilesAnimationSettings& rhs)
{
    auto tieStruct = [](const MultipleFilesAnimationSettings& settings)
    {
        return std::tie(settings.animationType, settings.firstTexturePath, settings.numberOfTextures,
                        settings.timeBetweenTexturesInSeconds, settings.loopsAllowed,
                        settings.interruptionAllowed);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::ostream& operator<<(std::ostream& os, const MultipleFilesAnimationSettings& animationSettings)
{
    return os << "animationType: " << animationSettings.animationType
              << " firstTexturePath: " << animationSettings.firstTexturePath
              << " numberOfTextures: " << animationSettings.numberOfTextures
              << " timeBetweenTexturesInSeconds: " << animationSettings.timeBetweenTexturesInSeconds
              << " loopsAllowed: " << animationSettings.loopsAllowed
              << " interruptionAllowed: " << animationSettings.interruptionAllowed;
}
}