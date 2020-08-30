#include "AnimationType.h"

#include "gtest/gtest.h"

using namespace graphics;
using namespace animation;
using namespace ::testing;

class AnimationTypeTest : public Test
{
public:
    const AnimationType animationType1 = AnimationType::Idle;
    const AnimationType animationType2 = AnimationType::Walk;
    const AnimationType animationType3 = AnimationType::Jump;
    const std::string animationTypeString1 = "idle";
    const std::string animationTypeString2 = "walk";
    const std::string animationTypeString3 = "jump";
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

TEST_F(AnimationTypeTest, givenStringCorrespondingToAnimationType_shouldThrowAnimationTypeNotFound)
{
    ASSERT_EQ(toAnimationType(animationTypeString1), animationType1);
    ASSERT_EQ(toAnimationType(animationTypeString2), animationType2);
    ASSERT_EQ(toAnimationType(animationTypeString3), animationType3);
}