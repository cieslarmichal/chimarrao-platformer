#pragma once

#include <string>
#include <vector>

#include "AnimationSettings.h"

namespace graphics::animation
{
struct AnimatorSettings
{
    std::string animatorName;
    std::vector<AnimationSettings> animationsSettings;
};

inline bool operator==(const AnimatorSettings& lhs, const AnimatorSettings& rhs)
{
    auto tieStruct = [](const AnimatorSettings& settings) {
        return std::tie(settings.animatorName, settings.animationsSettings);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}

using AnimatorsSettings = std::vector<AnimatorSettings>;
}