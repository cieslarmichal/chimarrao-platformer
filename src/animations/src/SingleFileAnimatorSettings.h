#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "SingleFileAnimationSettings.h"
#include "StlOperators.h"

namespace animations
{
struct SingleFileAnimatorSettings
{
    std::string animatorName;
    std::vector<SingleFileAnimationSettings> animationsSettings;
};

inline bool operator==(const SingleFileAnimatorSettings& lhs, const SingleFileAnimatorSettings& rhs)
{
    auto tieStruct = [](const SingleFileAnimatorSettings& settings) {
      return std::tie(settings.animatorName, settings.animationsSettings);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::ostream& operator<<(std::ostream& os, const SingleFileAnimatorSettings& animatorSettings)
{
    return os << "animatorName: " << animatorSettings.animatorName
              << " animationsSettings: " << animatorSettings.animationsSettings;
}

}