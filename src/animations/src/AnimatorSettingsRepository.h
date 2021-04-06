#pragma once

#include <boost/optional.hpp>

#include "MultipleFilesAnimatorSettings.h"

namespace animations
{
class AnimatorSettingsRepository
{
public:
    virtual ~AnimatorSettingsRepository() = default;

    virtual boost::optional<SingleFileAnimatorSettings>
    getSingleFileAnimatorSettings(const std::string& animatorName) const = 0;
    virtual boost::optional<MultipleFilesAnimatorSettings>
    getMultipleFileAnimatorSettings(const std::string& animatorName) const = 0;
};
}
