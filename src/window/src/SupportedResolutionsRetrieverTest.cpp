#include "SupportedResolutionsRetriever.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace window;

namespace
{
const std::vector<Resolution> expectedSupportedResolutions{{800, 600},  {1024, 768}, {1280, 720},
                                                           {1368, 768}, {1600, 900}, {1920, 1080}};
}

TEST(SupportedResolutionsRetrieverTest, shouldRetrieveSupportedResolutions)
{
    const auto actualSupportedResolutions = SupportedResolutionsRetriever::retrieveSupportedResolutions();

    ASSERT_EQ(actualSupportedResolutions, expectedSupportedResolutions);
}