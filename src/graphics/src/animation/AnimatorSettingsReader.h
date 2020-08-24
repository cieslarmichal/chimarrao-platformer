#pragma once

#include <boost/optional.hpp>

#include "AnimatorSettings.h"
#include "FilePath.h"

namespace graphics::animation
{
class AnimatorSettingsReader
{
public:
    virtual ~AnimatorSettingsReader() = default;

    virtual boost::optional<AnimatorsSettings> readAnimatorsSettings(const utils::FilePath&) const = 0;
};
}