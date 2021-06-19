#pragma once

#include <string>
#include <tuple>

namespace game
{
struct GridButtonInfo
{
    std::string buttonName;
    unsigned iconIndex;
    bool horizontalMoveCauseAction;
    bool verticalMoveKeepButtonSelected;
};

inline bool operator==(const GridButtonInfo& lhs, const GridButtonInfo& rhs)
{
    const auto tieStruct = [](const GridButtonInfo& gridButtonInfo)
    {
        return std::tie(gridButtonInfo.buttonName, gridButtonInfo.iconIndex,
                        gridButtonInfo.horizontalMoveCauseAction,
                        gridButtonInfo.verticalMoveKeepButtonSelected);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}
}