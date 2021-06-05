#pragma once

#include <ostream>
#include <string>
#include <tuple>

#include "Rect.h"
#include "TexturePath.h"
#include "Vector.h"

namespace animations
{
struct SingleFileAnimationSettings
{
    std::string animationType;
    graphics::TexturePath texturePath;
    utils::Vector2u textureSize;
    utils::IntRect textureRect;
    int numberOfTextures;
    float timeBetweenTexturesInSeconds;
    bool loopsAllowed;
    bool interruptionAllowed;
};

inline bool operator==(const SingleFileAnimationSettings& lhs, const SingleFileAnimationSettings& rhs)
{
    auto tieStruct = [](const SingleFileAnimationSettings& settings)
    {
        return std::tie(settings.animationType, settings.texturePath, settings.textureSize,
                        settings.textureRect, settings.numberOfTextures,
                        settings.timeBetweenTexturesInSeconds, settings.loopsAllowed,
                        settings.interruptionAllowed);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::ostream& operator<<(std::ostream& os, const SingleFileAnimationSettings& animationSettings)
{
    return os << "animationType: " << animationSettings.animationType
              << " texturePath: " << animationSettings.texturePath
              << " textureSize: " << animationSettings.textureSize
              << " textureRect: " << animationSettings.textureRect
              << " numberOfTextures: " << animationSettings.numberOfTextures
              << " timeBetweenTexturesInSeconds: " << animationSettings.timeBetweenTexturesInSeconds
              << " loopsAllowed: " << animationSettings.loopsAllowed
              << " interruptionAllowed: " << animationSettings.interruptionAllowed;
}

}