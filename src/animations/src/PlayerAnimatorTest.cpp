#include "PlayerAnimator.h"

#include "gtest/gtest.h"

#include "AnimatorSettingsRepositoryMock.h"
#include "RendererPoolMock.h"

#include "GetProjectPath.h"
#include "GraphicsIdGenerator.h"
#include "exceptions/AnimationTypeNotSupported.h"
#include "exceptions/AnimationsFromSettingsNotFound.h"
#include "exceptions/AnimatorSettingsNotFound.h"
#include "exceptions/InvalidAnimatorSettings.h"

using namespace graphics;
using namespace animations;
using namespace ::testing;

class PlayerAnimatorTest_Base : public Test
{
public:
    PlayerAnimatorTest_Base()
    {
        expectAnimatorsSettingFirstTextureWithCreation();

        EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings("player"))
            .WillRepeatedly(Return(animatorSettings));
    }

    void expectAnimatorsSettingFirstTextureWithCreation()
    {
        EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTextureRect, scaleRightDirection));
        EXPECT_CALL(*rendererPool, setTexture(graphicsId2, firstIdleTextureRect, scaleRightDirection));
    }

    const GraphicsId graphicsId1{GraphicsIdGenerator::generateId()};
    const GraphicsId graphicsId2{GraphicsIdGenerator::generateId()};
    const std::vector<MultipleFilesAnimationSettings> emptyAnimationsSettings{};
    const utils::DeltaTime timeNotExceedingTimeBetweenTextures{1.0};
    const utils::DeltaTime timeExceedingTimeBetweenTextures{2.5};
    const utils::Vector2f scaleRightDirection{1.0f, 1.0f};
    const utils::Vector2f scaleLeftDirection{-1.0f, 1.0f};
    const std::vector<MultipleFilesAnimationSettings> animationsSettings{{"idle", "idle/x1.txt", 3, 1.2f},
                                                                         {"walk", "walk/123.txt", 2, 2.0f}};
    const std::string projectPath{utils::getProjectPath("chimarrao-platformer")};
    const TextureRect firstIdleTextureRect{projectPath + "idle/x1.txt"};
    const TextureRect secondIdleTextureRect{projectPath + "idle/x2.txt"};
    const TextureRect firstWalkTextureRect{projectPath + "walk/123.txt"};
    const MultipleFilesAnimatorSettings animatorSettingsWithDifferentName{"diffName", animationsSettings};
    const MultipleFilesAnimatorSettings animatorSettingsWithEmptyAnimationsSettings{"player",
                                                                                    emptyAnimationsSettings};
    const MultipleFilesAnimatorSettings animatorSettings{"player", animationsSettings};
    const AnimationType notSupportedAnimationType{AnimationType::Jump};
    const AnimationType supportedAnimationType{AnimationType::Walk};
    std::shared_ptr<RendererPoolMock> rendererPool = std::make_shared<StrictMock<RendererPoolMock>>();
    std::shared_ptr<StrictMock<AnimatorSettingsRepositoryMock>> animatorSettingsRepo =
        std::make_shared<StrictMock<AnimatorSettingsRepositoryMock>>();
};

class PlayerAnimatorTest : public PlayerAnimatorTest_Base
{
public:
    PlayerAnimator playerAnimator{graphicsId1, rendererPool, animatorSettingsRepo, AnimationType::Idle};
    PlayerAnimator playerAnimatorWithLeftInitialDirection{graphicsId2, rendererPool, animatorSettingsRepo,
                                                          AnimationType::Idle, AnimationDirection::Left};
};

TEST_F(PlayerAnimatorTest, givenNoneAnimatorSettings_shouldThrowInvalidAnimatorSettingsNotFound)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings("player"))
        .WillOnce(Return(boost::none));

    ASSERT_THROW(PlayerAnimator(graphicsId1, rendererPool, animatorSettingsRepo),
                 animations::exceptions::AnimatorSettingsNotFound);
}

TEST_F(PlayerAnimatorTest,
       givenAnimatorSettingsWithDifferentNameThanPlayer_shouldThrowInvalidAnimatorConfigFile)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings("player"))
        .WillOnce(Return(animatorSettingsWithDifferentName));

    ASSERT_THROW(PlayerAnimator(graphicsId1, rendererPool, animatorSettingsRepo),
                 animations::exceptions::InvalidAnimatorSettings);
}

TEST_F(PlayerAnimatorTest,
       givenInitialAnimationTypeDifferentThanPlayersAnimationsType_shouldThrowAnimationTypeNotSupported)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings("player"))
        .WillOnce(Return(animatorSettings));

    ASSERT_THROW(PlayerAnimator(graphicsId1, rendererPool, animatorSettingsRepo, AnimationType::Jump),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(PlayerAnimatorTest, givenAnimatorSettingsWithEmptyAnimations_shouldThrowAnimationsFromSettingsNotFound)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings("player"))
        .WillOnce(Return(animatorSettingsWithEmptyAnimationsSettings));

    ASSERT_THROW(PlayerAnimator(graphicsId1, rendererPool, animatorSettingsRepo),
                 animations::exceptions::AnimationsFromSettingsNotFound);
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
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, secondIdleTextureRect, scaleRightDirection));

    const auto animationChanged = playerAnimator.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(PlayerAnimatorTest,
       givenTimeExceedingTimeBetweenTexturesWithLeftDirectedAnimation_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId2, secondIdleTextureRect, scaleLeftDirection));

    const auto animationChanged =
        playerAnimatorWithLeftInitialDirection.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(PlayerAnimatorTest,
       givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationTypeSet_shouldUpdateAnimation)
{
    playerAnimator.setAnimation(AnimationType::Walk);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstWalkTextureRect, scaleRightDirection));

    const auto animationChanged = playerAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(PlayerAnimatorTest,
       givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationDirectionSet_shouldUpdateAnimation)
{
    playerAnimator.setAnimationDirection(AnimationDirection::Left);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTextureRect, scaleLeftDirection));

    const auto animationChanged = playerAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(PlayerAnimatorTest, givenNotSupportedAnimationType_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(playerAnimator.setAnimation(notSupportedAnimationType),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(PlayerAnimatorTest, givenSupportedAnimationType_shouldNotThrowAndChangeAnimationType)
{
    ASSERT_NO_THROW(playerAnimator.setAnimation(supportedAnimationType));
    ASSERT_EQ(playerAnimator.getAnimationType(), supportedAnimationType);
}

TEST_F(PlayerAnimatorTest, givenNotSupportedAnimationTypeAndDirection_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(playerAnimator.setAnimation(notSupportedAnimationType, AnimationDirection::Right),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(PlayerAnimatorTest,
       givenSupportedAnimationTypeAndDirection_shouldNotThrowAndChangeAnimationTypeAndDirection)
{
    ASSERT_NO_THROW(playerAnimator.setAnimation(supportedAnimationType, AnimationDirection::Right));

    ASSERT_EQ(playerAnimator.getAnimationType(), supportedAnimationType);
    ASSERT_EQ(playerAnimator.getAnimationDirection(), AnimationDirection::Right);
}
