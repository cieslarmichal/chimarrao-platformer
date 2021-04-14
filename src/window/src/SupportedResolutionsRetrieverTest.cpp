#include "SupportedResolutionsRetriever.h"

#include <algorithm>

#include "gtest/gtest.h"

using namespace ::testing;
using namespace window;

class SupportedResolutionsRetrieverTest : public Test
{
public:
    const std::set<Resolution> commonResolutions{{640, 480},   {800, 600},   {1280, 720},
                                                 {1920, 1080}, {1920, 1440}, {3840, 2160}};
};

TEST_F(SupportedResolutionsRetrieverTest, shouldRetrieveSupportedResolutions)
{
    const auto actualSupportedResolutions = SupportedResolutionsRetriever::retrieveSupportedResolutions();

    ASSERT_TRUE(std::is_sorted(actualSupportedResolutions.begin(), actualSupportedResolutions.end()));
    ASSERT_TRUE(std::all_of(actualSupportedResolutions.begin(), actualSupportedResolutions.end(),
                            [&](const auto& actualResolution)
                            { return commonResolutions.contains(actualResolution); }));
}

TEST_F(SupportedResolutionsRetrieverTest, shouldRetrieveLowestResolution)
{
    const auto allSupportedResolutions = SupportedResolutionsRetriever::retrieveSupportedResolutions();

    const auto actualLowestResolution = SupportedResolutionsRetriever::retrieveLowestResolution();

    ASSERT_EQ(actualLowestResolution,
              *std::min_element(allSupportedResolutions.begin(), allSupportedResolutions.end()));
}

TEST_F(SupportedResolutionsRetrieverTest, shouldRetrieveHighestResolution)
{
    const auto allSupportedResolutions = SupportedResolutionsRetriever::retrieveSupportedResolutions();

    const auto actualHighestResolution = SupportedResolutionsRetriever::retrieveHighestResolution();

    ASSERT_EQ(actualHighestResolution,
              *std::max_element(allSupportedResolutions.begin(), allSupportedResolutions.end()));
}