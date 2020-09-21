#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "AnimationSettings.h"
#include "StlOperators.h"

namespace animations
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

inline std::ostream& operator<<(std::ostream& os, const AnimatorSettings& animatorSettings)
{
    return os << "animatorName: " << animatorSettings.animatorName
              << " animationsSettings: " << animatorSettings.animationsSettings;
}

using AnimatorsSettings = std::vector<AnimatorSettings>;
}