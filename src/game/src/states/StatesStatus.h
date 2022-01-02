#pragma once

#include <string>
#include <unordered_map>

#include "exceptions/StatesStatusNotFound.h"

namespace game
{
enum class StatesStatus
{
    Running,
    Exit
};

inline std::string toString(StatesStatus statesStatus)
{
    std::unordered_map<StatesStatus, std::string> statesStatusToString{{StatesStatus::Running, "Running"},
                                                                       {StatesStatus::Exit, "Exit"}};

    try
    {
        return statesStatusToString.at(statesStatus);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::StatesStatusNotFound{e.what()};
    }
}

inline std::ostream& operator<<(std::ostream& os, StatesStatus statesStatus)
{
    return os << "StatesStatus::" << toString(statesStatus);
}
}