#pragma once

#include <set>
#include <vector>

#include "Resolution.h"

namespace window
{
class SupportedResolutionsRetriever
{
public:
    static std::vector<Resolution> retrieveSupportedResolutions();
    static Resolution retrieveLowestResolution();
    static Resolution retrieveHighestResolution();

private:
    static std::vector<Resolution> getAllSupportedResolutions();

    static const std::set<Resolution> commonResolutions;
};
}