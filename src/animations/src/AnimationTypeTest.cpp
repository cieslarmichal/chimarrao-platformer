#include "AnimationType.h"

#include "gtest/gtest.h"

using namespace animations;
using namespace ::testing;

class AnimationTypeTest : public Test
{
public:
    const AnimationType animationType1 = AnimationType::Idle;
    const AnimationType animationType2 = AnimationType::Walk;
    const AnimationType animationType3 = AnimationType::Jump;
    const std::string animationTypeString1 = "Idle";
    const std::string animationTypeString2 = "Walk";
    const std::string animationTypeString3 = "Jump";
    const std::string animationTypeDifferentSize = "IDLE";
    const std::string nonExistingAnimationTypeString = "asdasdas";
};

TEST_F(AnimationTypeTest, givenAnimationType_shouldReturnString)
{
    ASSERT_EQ(toString(animationType1), animationTypeString1);
    ASSERT_EQ(toString(animationType2), animationTypeString2);
    ASSERT_EQ(toString(animationType3), animationTypeString3);
}

TEST_F(AnimationTypeTest, givenStringNotCorrespondingToAnimationType_shouldThrowAnimationTypeNotFound)
{
    ASSERT_THROW(toAnimationType(nonExistingAnimationTypeString), exceptions::AnimationTypeNotFound);
}

TEST_F(AnimationTypeTest,
       givenStringCorrespondingToAnimationTypeWithDifferentCaseSize_shouldReturnAnimationType)
{
    ASSERT_EQ(toAnimationType(animationTypeDifferentSize), animationType1);
}

TEST_F(AnimationTypeTest, givenStringCorrespondingToAnimationType_shouldReturnAnimationType)
{
    ASSERT_EQ(toAnimationType(animationTypeString1), animationType1);
    ASSERT_EQ(toAnimationType(animationTypeString2), animationType2);
    ASSERT_EQ(toAnimationType(animationTypeString3), animationType3);
}