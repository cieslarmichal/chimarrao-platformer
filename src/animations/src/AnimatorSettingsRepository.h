#pragma once

#include <boost/optional.hpp>

#include "AnimatorSettings.h"

namespace animations
{
class AnimatorSettingsRepository
{
public:
    virtual ~AnimatorSettingsRepository() = default;

    virtual boost::optional<AnimatorSettings> getAnimatorSettings(const std::string& animatorName) const = 0;
};
}
