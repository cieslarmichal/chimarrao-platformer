#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

#include "StringHelper.h"

namespace game
{
enum class NextState
{
    Same,
    Previous,
    Menu
};

inline std::string toString(NextState nextState)
{
    std::unordered_map<NextState, std::string> nextStateToString{
        {NextState::Same, "Same"}, {NextState::Previous, "Previous"}, {NextState::Menu, "Menu"}};

    return nextStateToString.at(nextState);
}

inline NextState toNextState(const std::string& nextStateAsString)
{
    const auto nextStateLowerString = utils::StringHelper::getLowerCases(nextStateAsString);

    std::unordered_map<std::string, NextState> stringToNextStates{
        {"same", NextState::Same}, {"previous", NextState::Previous}, {"menu", NextState::Menu}};

    return stringToNextStates.at(nextStateAsString);
}

inline std::ostream& operator<<(std::ostream& os, NextState nextState)
{
    return os << "NextState::" << toString(nextState);
}
}