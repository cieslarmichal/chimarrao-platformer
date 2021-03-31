#pragma once

#include "FilePath.h"
#include "AnimatorsSettings.h"

namespace animations
{
class AnimatorSettingsReader
{
public:
    virtual ~AnimatorSettingsReader() = default;

    virtual AnimatorsSettings readAnimatorsSettings(const utils::FilePath&) const = 0;
};
}