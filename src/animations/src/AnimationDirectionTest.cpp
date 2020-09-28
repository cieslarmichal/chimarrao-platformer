#include "AnimationDirection.h"

#include "gtest/gtest.h"

using namespace animations;
using namespace ::testing;

class AnimationDirectionTest : public Test
{
public:
    const AnimationDirection animationDirection1 = AnimationDirection::Left;
    const AnimationDirection animationDirection2 = AnimationDirection::Right;
    const std::string animationDirectionString1 = "Left";
    const std::string animationDirectionString2 = "Right";
    const std::string nonExistingAnimationDirectionString = "asdasdas";
    const std::string animationDirectionDifferentSize = "LEFT";
};

TEST_F(AnimationDirectionTest, givenAnimationDirection_shouldReturnString)
{
    ASSERT_EQ(toString(animationDirection1), animationDirectionString1);
    ASSERT_EQ(toString(animationDirection2), animationDirectionString2);
}

TEST_F(AnimationDirectionTest,
       givenStringNotCorrespondingToAnimationDirectionr_shouldThrowAnimationDirectionNotFound)
{
    ASSERT_THROW(toAnimationDirection(nonExistingAnimationDirectionString),
                 exceptions::AnimationDirectionNotFound);
}

TEST_F(AnimationDirectionTest,
       givenStringCorrespondingToAnimationDirectionWithDifferentCaseSize_shouldReturnAnimationDirection)
{
    ASSERT_EQ(toAnimationDirection(animationDirectionDifferentSize), animationDirection1);
}

TEST_F(AnimationDirectionTest, givenStringCorrespondingToAnimationDirection_shouldReturnAnimationDirection)
{
    ASSERT_EQ(toAnimationDirection(animationDirectionString1), animationDirection1);
    ASSERT_EQ(toAnimationDirection(animationDirectionString2), animationDirection2);
}