#include "SupportedResolutionsRetriever.h"

namespace window
{
const std::vector<Resolution> SupportedResolutionsRetriever::supportedResolutions{
    {1920, 1080}, {1600, 900}, {1368, 768}, {1280, 720}, {1024, 768}, {800, 600}};

std::vector<Resolution> SupportedResolutionsRetriever::retrieveSupportedResolutions()
{
    return supportedResolutions;
}

}