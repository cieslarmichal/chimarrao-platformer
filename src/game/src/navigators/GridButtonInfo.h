#pragma once

#include <iostream>
#include <string>
#include <tuple>

namespace game
{
struct GridButtonInfo
{
    std::string buttonName;
    unsigned iconIndex;
    bool horizontalMoveCauseAction{false};
    bool verticalMoveKeepButtonSelected{false};
    bool highlightsWhenSelected{true};
};

inline bool operator==(const GridButtonInfo& lhs, const GridButtonInfo& rhs)
{
    const auto tieStruct = [](const GridButtonInfo& gridButtonInfo)
    {
        return std::tie(gridButtonInfo.buttonName, gridButtonInfo.iconIndex,
                        gridButtonInfo.horizontalMoveCauseAction,
                        gridButtonInfo.verticalMoveKeepButtonSelected, gridButtonInfo.highlightsWhenSelected);
    };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::ostream& operator<<(std::ostream& os, const GridButtonInfo& gridButtonInfo)
{
    return os << "buttonName: " << gridButtonInfo.buttonName << " iconIndex: " << gridButtonInfo.iconIndex
              << " horizontalMoveCauseAction: " << gridButtonInfo.horizontalMoveCauseAction
              << " verticalMoveKeepButtonSelected: " << gridButtonInfo.verticalMoveKeepButtonSelected
              << " highlightsWhenSelected: " << gridButtonInfo.highlightsWhenSelected;
}
}