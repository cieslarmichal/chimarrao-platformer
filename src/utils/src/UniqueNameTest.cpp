#include "UniqueName.h"

#include "gtest/gtest.h"

#include "exceptions/NameIsNotUnique.h"

using namespace utils;
using namespace ::testing;

TEST(UniqueNameTest, createTwoSameNames_shouldThrowNameIsNotUnique)
{
    const auto name = UniqueName{"name"};
    ASSERT_THROW(UniqueName{"name"}, exceptions::NameIsNotUnique);
}

TEST(UniqueNameTest, destructedName_shouldBeAccessibleAgain)
{
    {
        const auto name = UniqueName{"name"};
    }
    ASSERT_NO_THROW(UniqueName{"name"});
}

TEST(UniqueNameTest, createWithDefaultConstructor_shouldGenerateId)
{
    const auto name = UniqueName{};

    ASSERT_FALSE(name.getName().empty());
}