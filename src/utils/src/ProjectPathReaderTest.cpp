#include "ProjectPathReader.h"

#include "boost/algorithm/string/predicate.hpp"
#include "gtest/gtest.h"

#include "exceptions/FileNotFound.h"

using namespace ::testing;
using namespace utils;

namespace
{
#ifdef _WIN32
const std::string validPathEnd{"chimarrao-platformer\\"};
#else
const std::string validPathEnd{"chimarrao-platformer/"};
#endif

}

TEST(GetProjectPathTest, shouldReturnProjectPath)
{
    const auto actualProjectPath = ProjectPathReader::getProjectRootPath();

    ASSERT_TRUE(boost::algorithm::ends_with(actualProjectPath, validPathEnd));
}
