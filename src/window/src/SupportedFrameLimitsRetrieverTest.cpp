#include "SupportedFrameLimitsRetriever.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace window;

namespace
{
const std::vector<unsigned int> expectedSupportedFrameLimits{30, 60, 90, 120};
}

TEST(SupportedFrameLimitsRetrieverTest, shouldRetrieveSupportedFrameLimits)
{
    const auto actualSupportedFrameLimits = SupportedFrameLimitsRetriever::retrieveSupportedFrameLimits();

    ASSERT_EQ(actualSupportedFrameLimits, expectedSupportedFrameLimits);
}