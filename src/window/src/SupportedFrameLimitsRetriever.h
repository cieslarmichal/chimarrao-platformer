#pragma once

#include <vector>

namespace window
{
class SupportedFrameLimitsRetriever
{
public:
    static std::vector<unsigned int> retrieveSupportedFrameLimits();

private:
    static const std::vector<unsigned int> supportedFrameLimits;
};
}