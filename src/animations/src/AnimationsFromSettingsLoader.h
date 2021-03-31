#pragma once

#include <unordered_map>

#include "Animation.h"
#include "AnimationType.h"
#include "MultipleFilesAnimationSettings.h"
#include "SingleFileAnimationSettings.h"

namespace animations
{
class AnimationsFromSettingsLoader
{
public:
    static void loadAnimationsFromMultipleFilesAnimationsSettings(std::unordered_map<AnimationType, Animation>&,
                                           const std::vector<MultipleFilesAnimationSettings>&);
    static void loadAnimationsFromSingleFileAnimationsSettings(std::unordered_map<AnimationType, Animation>&,
                                           const std::vector<SingleFileAnimationSettings>&);
};
}