#include "PlayerAnimator.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "GetProjectPath.h"
#include "GraphicsIdGenerator.h"
#include "animation/exceptions/AnimationTypeNotSupported.h"
#include "animation/exceptions/AnimationsFromSettingsNotFound.h"
#include "animation/exceptions/InvalidAnimatorSettings.h"

using namespace graphics;
using namespace animation;
using namespace ::testing;

class PlayerAnimatorTest_Base : public Test
{
public:
    PlayerAnimatorTest_Base()
    {
        expectAnimatorsSettingFirstTextureWithCreation();
    }

    void expectAnimatorsSettingFirstTextureWithCreation()
    {
        EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTexturePath, scaleRightDirection));
        EXPECT_CALL(*rendererPool, setTexture(graphicsId2, firstIdleTexturePath, scaleRightDirection));
    }

    const GraphicsId graphicsId1{GraphicsIdGenerator::generateId()};
    const GraphicsId graphicsId2{GraphicsIdGenerator::generateId()};
    const AnimationsSettings emptyAnimationsSettings{};
    const utils::DeltaTime timeNotExceedingTimeBetweenTextures{1.0};
    const utils::DeltaTime timeExceedingTimeBetweenTextures{2.5};
    const utils::Vector2f scaleRightDirection{1.0, 1.0};
    const utils::Vector2f scaleLeftDirection{-1.0, 1.0};
    const AnimationsSettings animationsSettings{{"idle", "idle/x1.txt", 3, 1.2},
                                                {"walk", "walk/123.txt", 2, 2.0}};
    const std::string projectPath{utils::getProjectPath("chimarrao-platformer")};
    const TexturePath firstIdleTexturePath{projectPath + "idle/x1.txt"};
    const TexturePath secondIdleTexturePath{projectPath + "idle/x2.txt"};
    const TexturePath firstWalkTexturePath{projectPath + "walk/123.txt"};
    const TexturePath secondWalkTexturePath{projectPath + "walk/124.txt"};
    const AnimatorSettings animatorSettingsWithDifferentName{"diffName", animationsSettings};
    const AnimatorSettings animatorSettingsWithEmptyAnimationsSettings{"player", emptyAnimationsSettings};
    const AnimatorSettings animatorSettings{"player", animationsSettings};
    const AnimationType notSupportedAnimationType{AnimationType::Jump};
    const AnimationType supportedAnimationType{AnimationType::Walk};
    std::shared_ptr<RendererPoolMock> rendererPool = std::make_shared<StrictMock<RendererPoolMock>>();
};

class PlayerAnimatorTest : public PlayerAnimatorTest_Base
{
public:
    PlayerAnimator playerAnimator{graphicsId1, rendererPool, animatorSettings, AnimationType::Idle};
    PlayerAnimator playerAnimatorWithLeftInitialDirection{graphicsId2, rendererPool, animatorSettings,
                                                          AnimationType::Idle, AnimationDirection::Left};
};

TEST_F(PlayerAnimatorTest,
       givenAnimatorSettingsWithDifferentNameThanPlayer_shouldThrowInvalidAnimatorConfigFile)
{
    ASSERT_THROW(PlayerAnimator(graphicsId1, rendererPool, animatorSettingsWithDifferentName),
                 exceptions::InvalidAnimatorSettings);
}

TEST_F(PlayerAnimatorTest,
       givenInitialAnimationTypeDifferentThanPlayersAnimationsType_shouldThrowAnimationTypeNotSupported)
{
    ASSERT_THROW(PlayerAnimator(graphicsId1, rendererPool, animatorSettings, AnimationType::Jump),
                 exceptions::AnimationTypeNotSupported);
}

TEST_F(PlayerAnimatorTest, givenAnimatorSettingsWithEmptyAnimations_shouldThrowAnimationsFromSettingsNotFound)
{
    ASSERT_THROW(PlayerAnimator(graphicsId1, rendererPool, animatorSettingsWithEmptyAnimationsSettings),
                 exceptions::AnimationsFromSettingsNotFound);
}

TEST_F(PlayerAnimatorTest, getInitialAnimationType_shouldReturnIdle)
{
    ASSERT_EQ(playerAnimator.getAnimationType(), AnimationType::Idle);
}

TEST_F(PlayerAnimatorTest, getInitialDefaultAnimationDirection_shouldReturnAnimationDirectionRight)
{
    ASSERT_EQ(playerAnimator.getAnimationDirection(), AnimationDirection::Right);
}

TEST_F(PlayerAnimatorTest, shouldSetAnimationType)
{
    playerAnimator.setAnimation(AnimationType::Walk);

    ASSERT_EQ(playerAnimator.getAnimationType(), AnimationType::Walk);
}

TEST_F(PlayerAnimatorTest, shouldSetAnimationDirection)
{
    playerAnimator.setAnimationDirection(AnimationDirection::Left);

    ASSERT_EQ(playerAnimator.getAnimationDirection(), AnimationDirection::Left);
}

TEST_F(PlayerAnimatorTest, givenTimeNotExceedingTimeBetweenTextures_shouldNotUpdateAnimation)
{
    const auto animationChanged = playerAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_FALSE(animationChanged);
}

TEST_F(PlayerAnimatorTest, givenTimeExceedingTimeBetweenTextures_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, secondIdleTexturePath, scaleRightDirection));

    const auto animationChanged = playerAnimator.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(PlayerAnimatorTest,
       givenTimeExceedingTimeBetweenTexturesWithLeftDirectedAnimation_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId2, secondIdleTexturePath, scaleLeftDirection));

    const auto animationChanged = playerAnimatorWithLeftInitialDirection.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(PlayerAnimatorTest, givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationTypeSet_shouldUpdateAnimation)
{
    playerAnimator.setAnimation(AnimationType::Walk);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstWalkTexturePath, scaleRightDirection));

    const auto animationChanged = playerAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(PlayerAnimatorTest, givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationDirectionSet_shouldUpdateAnimation)
{
    playerAnimator.setAnimationDirection(AnimationDirection::Left);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTexturePath, scaleLeftDirection));

    const auto animationChanged = playerAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(PlayerAnimatorTest, givenNotSupportedAnimationType_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(playerAnimator.setAnimation(notSupportedAnimationType),
                 exceptions::AnimationTypeNotSupported);
}

TEST_F(PlayerAnimatorTest, givenSupportedAnimationType_shouldNotThrowAndChangeAnimationType)
{
    ASSERT_NO_THROW(playerAnimator.setAnimation(supportedAnimationType));
    ASSERT_EQ(playerAnimator.getAnimationType(), supportedAnimationType);
}

TEST_F(PlayerAnimatorTest, givenNotSupportedAnimationTypeAndDirection_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(playerAnimator.setAnimation(notSupportedAnimationType, AnimationDirection::Right),
                 exceptions::AnimationTypeNotSupported);
}

TEST_F(PlayerAnimatorTest, givenSupportedAnimationTypeAndDirection_shouldNotThrowAndChangeAnimationTypeAndDirection)
{
    ASSERT_NO_THROW(playerAnimator.setAnimation(supportedAnimationType, AnimationDirection::Right));

    ASSERT_EQ(playerAnimator.getAnimationType(), supportedAnimationType);
    ASSERT_EQ(playerAnimator.getAnimationDirection(), AnimationDirection::Right);
}
