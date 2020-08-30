#pragma once

#include <unordered_map>

#include "Animation.h"
#include "AnimationSettings.h"
#include "AnimationType.h"

namespace graphics::animation
{
class AnimationsFromSettingsLoader
{
public:
    static void loadAnimationsFromSettings(std::unordered_map<AnimationType, Animation>&,
                                           const AnimationsSettings&);
};
}