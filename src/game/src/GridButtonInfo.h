#pragma once

#include <string>

namespace game
{
struct GridButtonInfo
{
    std::string buttonName;
    unsigned iconIndex;
    bool horizontalMoveCauseAction;
};

inline bool operator==(const GridButtonInfo& lhs, const GridButtonInfo& rhs)
{
    auto tieStruct = [](const GridButtonInfo& gridButtonInfo)
    {
        return std::tie(gridButtonInfo.buttonName, gridButtonInfo.iconIndex,
                        gridButtonInfo.horizontalMoveCauseAction);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}
}