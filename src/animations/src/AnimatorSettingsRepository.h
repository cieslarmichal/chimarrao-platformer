#pragma once

#include <boost/optional.hpp>

#include "AnimationsApi.h"
#include "MultipleFilesAnimatorSettings.h"
#include "SingleFileAnimatorSettings.h"

namespace animations
{
class ANIMATIONS_API AnimatorSettingsRepository
{
public:
    virtual ~AnimatorSettingsRepository() = default;

    virtual boost::optional<SingleFileAnimatorSettings>
    getSingleFileAnimatorSettings(const std::string& animatorName) const = 0;
    virtual boost::optional<MultipleFilesAnimatorSettings>
    getMultipleFileAnimatorSettings(const std::string& animatorName) const = 0;
};
}
