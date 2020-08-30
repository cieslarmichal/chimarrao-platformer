#include "PlayerAnimator.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"

#include "GetProjectPath.h"
#include "GraphicsIdGenerator.h"
#include "animation/exceptions/AnimationTypeNotSupported.h"
#include "animation/exceptions/AnimationsFromSettingsNotFound.h"
#include "animation/exceptions/InvalidAnimatorConfigFile.h"

using namespace graphics;
using namespace animation;
using namespace ::testing;

class PlayerAnimatorTest : public Test
{
public:
    const GraphicsId graphicsId{GraphicsIdGenerator::generateId()};
    const AnimationsSettings emptyAnimationsSettings{};
    const utils::DeltaTime timeNotExceedingTimeBetweenTextures{1.0};
    const utils::DeltaTime timeExceedingTimeBetweenTextures{2.5};
    const utils::Vector2f scaleRightDirection{1.5, 1.5};
    const utils::Vector2f scaleLeftDirection{-1.5, 1.5};
    const AnimationsSettings animationsSettings{{"idle", "dev/x1.txt", 3, 1.2},
                                                {"walk", "elo/123.txt", 2, 2.0}};
    const std::string projectPath{utils::getProjectPath("chimarrao")};
    const TexturePath secondIdleTexturePath{projectPath + "dev/x2.txt"};
    const TexturePath secondWalkTexturePath{projectPath + "elo/124.txt"};
    const AnimatorSettings animatorSettingsWithDifferentName{"diffName", animationsSettings};
    const AnimatorSettings animatorSettingsWithEmptyAnimationsSettings{"player", emptyAnimationsSettings};
    const AnimatorSettings animatorSettings{"player", animationsSettings};
    const AnimationType notSupportedAnimationType{AnimationType::Jump};
    const AnimationType supportedAnimationType{AnimationType::Walk};
    std::shared_ptr<RendererPoolMock> rendererPool = std::make_shared<StrictMock<RendererPoolMock>>();
    PlayerAnimator playerAnimator{graphicsId, rendererPool, animatorSettings, AnimationType::Idle};
    PlayerAnimator playerAnimatorWithLeftInitialDirection{graphicsId, rendererPool, animatorSettings,
                                                          AnimationType::Idle, AnimationDirection::Left};
};

TEST_F(PlayerAnimatorTest,
       givenAnimatorSettingsWithDifferentNameThanPlayer_shouldThrowInvalidAnimatorConfigFile)
{
    ASSERT_THROW(PlayerAnimator(graphicsId, rendererPool, animatorSettingsWithDifferentName),
                 exceptions::InvalidAnimatorConfigFile);
}

TEST_F(PlayerAnimatorTest,
       givenInitialAnimationTypeDifferentThanPlayersAnimationsType_shouldThrowAnimationTypeNotSupported)
{
    ASSERT_THROW(PlayerAnimator(graphicsId, rendererPool, animatorSettings, AnimationType::Jump),
                 exceptions::AnimationTypeNotSupported);
}

TEST_F(PlayerAnimatorTest, givenAnimatorSettingsWithEmptyAnimations_shouldThrowAnimationsFromSettingsNotFound)
{
    ASSERT_THROW(PlayerAnimator(graphicsId, rendererPool, animatorSettingsWithEmptyAnimationsSettings),
                 exceptions::AnimationsFromSettingsNotFound);
}

TEST_F(PlayerAnimatorTest, givenTimeNotExceedingTimeBetweenTextures_shouldNotUpdateAnimation)
{
    playerAnimator.update(timeNotExceedingTimeBetweenTextures);
}

TEST_F(PlayerAnimatorTest, givenTimeExceedingTimeBetweenTextures_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId, secondIdleTexturePath, scaleRightDirection));

    playerAnimator.update(timeExceedingTimeBetweenTextures);
}

TEST_F(PlayerAnimatorTest,
       givenTimeExceedingTimeBetweenTexturesWithLeftDirectedAnimation_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId, secondIdleTexturePath, scaleLeftDirection));

    playerAnimatorWithLeftInitialDirection.update(timeExceedingTimeBetweenTextures);
}

TEST_F(PlayerAnimatorTest, givenNotSupportedAnimationType_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(playerAnimator.setAnimation(notSupportedAnimationType),
                 exceptions::AnimationTypeNotSupported);
}

TEST_F(PlayerAnimatorTest, givenSupportedAnimationType_shouldNotThrow)
{
    ASSERT_NO_THROW(playerAnimator.setAnimation(supportedAnimationType));
}

TEST_F(PlayerAnimatorTest, givenNotSupportedAnimationTypeAndDirection_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(playerAnimator.setAnimation(notSupportedAnimationType, AnimationDirection::Right),
                 exceptions::AnimationTypeNotSupported);
}

TEST_F(PlayerAnimatorTest, givenSupportedAnimationTypeAndDirection_shouldNotThrow)
{
    ASSERT_NO_THROW(playerAnimator.setAnimation(supportedAnimationType, AnimationDirection::Right));
}

TEST_F(
    PlayerAnimatorTest,
    givenChangedInitialAnimationType_andTimeNotExceedingNewAnimationTimeBetweenTextures_shouldNotUpdateTexturesOnChangedAnimation)
{
    playerAnimator.setAnimation(AnimationType::Walk);

    playerAnimator.update(timeNotExceedingTimeBetweenTextures);
}

TEST_F(
    PlayerAnimatorTest,
    givenChangedInitialAnimationType_andGivenTimeExceedingNewAnimationTimeBetweenTextures_shouldUpdateTexturesOnChangedAnimation)
{
    playerAnimator.setAnimation(AnimationType::Walk);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId, secondWalkTexturePath, scaleRightDirection));

    playerAnimator.update(timeExceedingTimeBetweenTextures);
}
