#include "InputKey.h"

#include "gtest/gtest.h"

using namespace input;
using namespace ::testing;

class InputKeyTest : public Test
{
public:
    const InputKey inputKey1 = InputKey::Left;
    const InputKey inputKey2 = InputKey::Up;
    const InputKey inputKey3 = InputKey::MouseRight;
    const std::string inputKeyString1 = "Left";
    const std::string inputKeyString2 = "Up";
    const std::string inputKeyString3 = "MouseRight";
    const std::string nonExistingInputKeyString = "asdasdas";
    const std::string inputKeyDifferentSize = "LEFT";
};

TEST_F(InputKeyTest, givenInputKey_shouldReturnString)
{
    ASSERT_EQ(toString(inputKey1), inputKeyString1);
    ASSERT_EQ(toString(inputKey2), inputKeyString2);
    ASSERT_EQ(toString(inputKey3), inputKeyString3);
}

TEST_F(InputKeyTest, givenStringNotCorrespondingToInputKey_shouldThrowInputKeyNotFound)
{
    ASSERT_THROW(toInputKey(nonExistingInputKeyString), exceptions::InputKeyNotFound);
}

TEST_F(InputKeyTest, givenStringCorrespondingToInputKeyWithDifferentCaseSize_shouldReturnInputKey)
{
    ASSERT_EQ(toInputKey(inputKeyDifferentSize), inputKey1);
}

TEST_F(InputKeyTest, givenStringCorrespondingToInputKey_shouldReturnInputKey)
{
    ASSERT_EQ(toInputKey(inputKeyString1), inputKey1);
    ASSERT_EQ(toInputKey(inputKeyString2), inputKey2);
    ASSERT_EQ(toInputKey(inputKeyString3), inputKey3);
}