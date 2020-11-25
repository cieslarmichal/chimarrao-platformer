#include "SupportedFrameLimitsRetriever.h"

namespace window
{
const std::vector<unsigned int> SupportedFrameLimitsRetriever::supportedFrameLimits{30, 60, 90, 120};

std::vector<unsigned int> SupportedFrameLimitsRetriever::retrieveSupportedFrameLimits()
{
    return supportedFrameLimits;
}
}