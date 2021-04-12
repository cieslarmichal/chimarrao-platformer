#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

#include "StringHelper.h"
#include "exceptions/AnimationDirectionNotFound.h"

namespace animations
{
enum class AnimationDirection
{
    Right,
    Left
};

inline std::string toString(AnimationDirection animationDirection)
{
    std::unordered_map<AnimationDirection, std::string> animationDirectionToString{
        {AnimationDirection::Right, "Right"}, {AnimationDirection::Left, "Left"}};

    try
    {
        return animationDirectionToString.at(animationDirection);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::AnimationDirectionNotFound{e.what()};
    }
}

inline AnimationDirection toAnimationDirection(const std::string& animationDirectionAsString)
{
    const auto animationDirectionLowerString = utils::StringHelper::getLowerCases(animationDirectionAsString);

    std::unordered_map<std::string, AnimationDirection> stringToAnimationDirection{
        {"right", AnimationDirection::Right}, {"left", AnimationDirection::Left}};

    try
    {
        return stringToAnimationDirection.at(animationDirectionLowerString);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::AnimationDirectionNotFound{e.what()};
    }
}

inline std::ostream& operator<<(std::ostream& os, AnimationDirection animationDirection)
{
    return os << "AnimationDirection::" << toString(animationDirection);
}
}