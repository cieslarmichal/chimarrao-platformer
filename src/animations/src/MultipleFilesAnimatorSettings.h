#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "MultipleFilesAnimationSettings.h"
#include "StlOperators.h"

namespace animations
{
struct MultipleFilesAnimatorSettings
{
    std::string animatorName;
    std::vector<MultipleFilesAnimationSettings> animationsSettings;
};

inline bool operator==(const MultipleFilesAnimatorSettings& lhs, const MultipleFilesAnimatorSettings& rhs)
{
    auto tieStruct = [](const MultipleFilesAnimatorSettings& settings) {
        return std::tie(settings.animatorName, settings.animationsSettings);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::ostream& operator<<(std::ostream& os, const MultipleFilesAnimatorSettings& animatorSettings)
{
    return os << "animatorName: " << animatorSettings.animatorName
              << " animationsSettings: " << animatorSettings.animationsSettings;
}

}