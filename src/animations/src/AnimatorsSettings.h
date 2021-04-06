#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "MultipleFilesAnimatorSettings.h"
#include "SingleFileAnimatorSettings.h"
#include "StlOperators.h"

namespace animations
{
struct AnimatorsSettings
{
    std::vector<SingleFileAnimatorSettings> singleFileAnimatorsSettings;
    std::vector<MultipleFilesAnimatorSettings> multipleFilesAnimatorSettings;
};

inline bool operator==(const AnimatorsSettings& lhs, const AnimatorsSettings& rhs)
{
    auto tieStruct = [](const AnimatorsSettings& settings) {
        return std::tie(settings.singleFileAnimatorsSettings, settings.multipleFilesAnimatorSettings);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::ostream& operator<<(std::ostream& os, const AnimatorsSettings& animatorsSettings)
{
    return os << "singleFileAnimatorsSettings: " << animatorsSettings.singleFileAnimatorsSettings
              << " multipleFilesAnimatorSettings: " << animatorsSettings.multipleFilesAnimatorSettings;
}

}