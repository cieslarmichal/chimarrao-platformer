#pragma once

#include <vector>

#include "Resolution.h"

namespace window
{
class SupportedResolutionsRetriever
{
public:
    static std::vector<Resolution> retrieveSupportedResolutions();

private:
    static const std::vector<Resolution> supportedResolutions;
};
}