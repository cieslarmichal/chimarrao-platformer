#include "SupportedResolutionsRetriever.h"

namespace window
{
const std::vector<Resolution> SupportedResolutionsRetriever::supportedResolutions{
    {800, 600}, {1024, 768}, {1280, 720}, {1368, 768}, {1600, 900}, {1920, 1080}};

std::vector<Resolution> SupportedResolutionsRetriever::retrieveSupportedResolutions()
{
    return supportedResolutions;
}

}