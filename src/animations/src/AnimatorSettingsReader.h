#pragma once

#include "AnimatorSettings.h"
#include "FilePath.h"

namespace animations
{
class AnimatorSettingsReader
{
public:
    virtual ~AnimatorSettingsReader() = default;

    virtual AnimatorsSettings readAnimatorsSettings(const utils::FilePath&) const = 0;
};
}