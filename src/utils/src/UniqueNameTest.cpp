#include "UniqueName.h"

#include "gtest/gtest.h"
#include "exceptions/NameIsNotUnique.h"

using namespace utils;
using namespace ::testing;

TEST(UniqueNameTest, createTwoSameNames_shouldThrowNameIsNotUnique)
{
    ASSERT_NO_THROW(UniqueName{"name"});
    ASSERT_THROW(UniqueName{"name"}, exceptions::NameIsNotUnique);
}
