#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

#include "StringHelper.h"
#include "exceptions/VisibilityLayerNotFound.h"

namespace graphics
{
enum class VisibilityLayer
{
    First,
    Second,
    Third,
    Background,
    Invisible
};

inline std::string toString(VisibilityLayer visibilityLayer)
{
    std::unordered_map<VisibilityLayer, std::string> visibilityLayerToString{
        {VisibilityLayer::First, "First"},
        {VisibilityLayer::Second, "Second"},
        {VisibilityLayer::Third, "Third"},
        {VisibilityLayer::Background, "Background"},
        {VisibilityLayer::Invisible, "Invisible"}};

    try
    {
        return visibilityLayerToString.at(visibilityLayer);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::VisibilityLayerNotFound{e.what()};
    }
}

inline VisibilityLayer toVisibilityLayer(const std::string& visibilityLayerAsString)
{
    const auto visibilityLayerLowerString = utils::getLowerCases(visibilityLayerAsString);

    std::unordered_map<std::string, VisibilityLayer> stringToVisibilityLayer{
        {"first", VisibilityLayer::First},
        {"second", VisibilityLayer::Second},
        {"third", VisibilityLayer::Third},
        {"background", VisibilityLayer::Background},
        {"invisible", VisibilityLayer::Invisible}};

    try
    {
        return stringToVisibilityLayer.at(visibilityLayerLowerString);
    }
    catch (const std::out_of_range& e)
    {
        throw exceptions::VisibilityLayerNotFound{e.what()};
    }
}

inline std::ostream& operator<<(std::ostream& os, VisibilityLayer visibilityLayer)
{
    return os << "VisibilityLayer::" << toString(visibilityLayer);
}
}