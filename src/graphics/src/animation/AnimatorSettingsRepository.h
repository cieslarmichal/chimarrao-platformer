#pragma once

#include "AnimatorSettings.h"
#include <boost/optional.hpp>

namespace graphics::animation
{
class AnimatorSettingsRepository
{
public:
    virtual ~AnimatorSettingsRepository() = default;

    virtual boost::optional<AnimatorSettings> getAnimatorSettings(const std::string& animatorName) const = 0;
};
}
