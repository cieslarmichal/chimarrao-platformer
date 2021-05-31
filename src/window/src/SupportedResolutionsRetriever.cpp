#include "SupportedResolutionsRetriever.h"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>

#include "StlOperators.h"
#include "exceptions/ResolutionsNotDetected.h"

namespace window
{
const std::set<Resolution> SupportedResolutionsRetriever::commonResolutions{
    {640, 480}, {800, 600}, {1280, 720}, {1920, 1080}, {1920, 1440}, {3840, 2160}};

std::vector<Resolution> SupportedResolutionsRetriever::retrieveSupportedResolutions()
{
    std::vector<Resolution> supportedCommonResolutions;
    for (const auto& supportedResolution : getAllSupportedResolutions())
    {
        if (std::any_of(commonResolutions.begin(), commonResolutions.end(),
                        [&](const Resolution& commonResolution)
                        { return supportedResolution == commonResolution; }))
        {
            supportedCommonResolutions.push_back(supportedResolution);
        }
    }
    std::sort(supportedCommonResolutions.begin(), supportedCommonResolutions.end(), std::greater<>());
    return supportedCommonResolutions;
}

Resolution SupportedResolutionsRetriever::retrieveLowestResolution()
{
    const auto supportedResolutions = retrieveSupportedResolutions();
    return *std::min_element(supportedResolutions.begin(), supportedResolutions.end());
}

Resolution SupportedResolutionsRetriever::retrieveHighestResolution()
{
    const auto supportedResolutions = retrieveSupportedResolutions();
    return *std::max_element(supportedResolutions.begin(), supportedResolutions.end());
}

std::set<Resolution> SupportedResolutionsRetriever::getAllSupportedResolutions()
{
    const auto allModes = sf::VideoMode::getFullscreenModes();
    if (allModes.empty())
    {
        throw exceptions::ResolutionsNotDetected{"Not found any supported resolutions"};
    }

    std::set<Resolution> allSupportedResolutions;
    for (const auto& mode : allModes)
    {
        allSupportedResolutions.insert({mode.width, mode.height});
    }
    return allSupportedResolutions;
}

}