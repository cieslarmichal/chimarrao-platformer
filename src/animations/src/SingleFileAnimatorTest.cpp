#include "SingleFileAnimator.h"

#include "gtest/gtest.h"

#include "AnimatorSettingsRepositoryMock.h"
#include "RendererPoolMock.h"

#include "GraphicsIdGenerator.h"
#include "ProjectPathReader.h"
#include "exceptions/AnimationTypeNotSupported.h"
#include "exceptions/AnimationsFromSettingsNotFound.h"
#include "exceptions/AnimatorSettingsNotFound.h"
#include "exceptions/InvalidAnimatorSettings.h"

using namespace graphics;
using namespace animations;
using namespace ::testing;

class SingleFileAnimatorTest_Base : public Test
{
public:
    SingleFileAnimatorTest_Base()
    {
        expectAnimatorsSettingFirstTextureWithCreation();

        EXPECT_CALL(*animatorSettingsRepo, getSingleFileAnimatorSettings(animatorName))
            .WillRepeatedly(Return(animatorSettings));
    }

    void expectAnimatorsSettingFirstTextureWithCreation()
    {
        EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTextureRect, scaleRightDirection));
        EXPECT_CALL(*rendererPool, setTexture(graphicsId2, firstIdleTextureRect, scaleRightDirection));
    }

    const std::string animatorName{"bunny"};
    const GraphicsId graphicsId1{GraphicsIdGenerator::generateId()};
    const GraphicsId graphicsId2{GraphicsIdGenerator::generateId()};
    const utils::DeltaTime timeNotExceedingTimeBetweenTextures{1.0};
    const utils::DeltaTime timeExceedingTimeBetweenTextures{2.5};
    const utils::Vector2f scaleRightDirection{1.0, 1.0};
    const utils::Vector2f scaleLeftDirection{-1.0, 1.0};
    const std::string projectPath{utils::ProjectPathReader::getProjectRootPath()};
    const SingleFileAnimationSettings bunnySingleFileIdleAnimationSettings{
        "idle",
        "resources/bunny-hop-spritesheet.png",
        utils::Vector2u{192, 128},
        utils::IntRect{40, 64, 40, 32},
        1,
        1.2f};
    const SingleFileAnimationSettings bunnySingleFileWalkAnimationSettings{
        "walk",
        "resources/bunny-hop-spritesheet.png",
        utils::Vector2u{192, 128},
        utils::IntRect{0, 32, 48, 32},
        4,
        2.0f};
    const TextureRect firstIdleTextureRect{projectPath + "resources/bunny-hop-spritesheet.png",
                                           utils::IntRect{40, 64, 40, 32}};
    const TextureRect firstWalkTextureRect{projectPath + "resources/bunny-hop-spritesheet.png",
                                           utils::IntRect{0, 32, 48, 32}};
    const std::vector<SingleFileAnimationSettings> bunnySingleFileAnimationsSettings{
        bunnySingleFileIdleAnimationSettings, bunnySingleFileWalkAnimationSettings};
    const SingleFileAnimatorSettings animatorSettings{"bunny", bunnySingleFileAnimationsSettings};
    const SingleFileAnimatorSettings animatorSettingsWithDifferentAnimatorName{
        "xxx", bunnySingleFileAnimationsSettings};
    const SingleFileAnimatorSettings animatorSettingsWithEmptyAnimations{animatorName, {}};
    const AnimationType notSupportedAnimationType{AnimationType::Jump};
    const AnimationType supportedAnimationType{AnimationType::Walk};
    std::shared_ptr<RendererPoolMock> rendererPool = std::make_shared<StrictMock<RendererPoolMock>>();
    std::shared_ptr<StrictMock<AnimatorSettingsRepositoryMock>> animatorSettingsRepo =
        std::make_shared<StrictMock<AnimatorSettingsRepositoryMock>>();
};

class SingleFileAnimatorTest : public SingleFileAnimatorTest_Base
{
public:
    SingleFileAnimator singleFileAnimator{graphicsId1, rendererPool, animatorSettingsRepo, animatorName,
                                          AnimationType::Idle};
    SingleFileAnimator singleFileAnimatorWithLeftInitialDirection{
        graphicsId2,  rendererPool,        animatorSettingsRepo,
        animatorName, AnimationType::Idle, AnimationDirection::Left};
};

TEST_F(SingleFileAnimatorTest, givenNoneAnimatorSettings_shouldThrowInvalidAnimatorSettingsNotFound)
{
    EXPECT_CALL(*animatorSettingsRepo, getSingleFileAnimatorSettings(animatorName))
        .WillOnce(Return(boost::none));

    ASSERT_THROW(SingleFileAnimator(graphicsId1, rendererPool, animatorSettingsRepo, animatorName,
                                    AnimationType::Idle),
                 animations::exceptions::AnimatorSettingsNotFound);
}

TEST_F(SingleFileAnimatorTest,
       givenAnimatorSettingsWithDifferentNameThanAnimator_shouldThrowInvalidAnimatorConfigFile)
{
    EXPECT_CALL(*animatorSettingsRepo, getSingleFileAnimatorSettings(animatorName))
        .WillOnce(Return(animatorSettingsWithDifferentAnimatorName));

    ASSERT_THROW(SingleFileAnimator(graphicsId1, rendererPool, animatorSettingsRepo, animatorName,
                                    AnimationType::Idle),
                 animations::exceptions::InvalidAnimatorSettings);
}

TEST_F(SingleFileAnimatorTest,
       givenInitialAnimationTypeDifferentThanAnimatorsAnimationsType_shouldThrowAnimationTypeNotSupported)
{
    EXPECT_CALL(*animatorSettingsRepo, getSingleFileAnimatorSettings(animatorName))
        .WillOnce(Return(animatorSettings));

    ASSERT_THROW(SingleFileAnimator(graphicsId1, rendererPool, animatorSettingsRepo, animatorName,
                                    AnimationType::Jump),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(SingleFileAnimatorTest,
       givenAnimatorSettingsWithEmptyAnimations_shouldThrowAnimationsFromSettingsNotFound)
{
    EXPECT_CALL(*animatorSettingsRepo, getSingleFileAnimatorSettings(animatorName))
        .WillOnce(Return(animatorSettingsWithEmptyAnimations));

    ASSERT_THROW(SingleFileAnimator(graphicsId1, rendererPool, animatorSettingsRepo, animatorName,
                                    AnimationType::Idle),
                 animations::exceptions::AnimationsFromSettingsNotFound);
}

TEST_F(SingleFileAnimatorTest, getInitialAnimationType_shouldReturnIdle)
{
    ASSERT_EQ(singleFileAnimator.getAnimationType(), AnimationType::Idle);
}

TEST_F(SingleFileAnimatorTest, getInitialDefaultAnimationDirection_shouldReturnAnimationDirectionRight)
{
    ASSERT_EQ(singleFileAnimator.getAnimationDirection(), AnimationDirection::Right);
}

TEST_F(SingleFileAnimatorTest, shouldSetWalkAnimationType)
{
    singleFileAnimator.setAnimation(AnimationType::Walk);
    ASSERT_EQ(singleFileAnimator.getAnimationType(), AnimationType::Walk);
}

TEST_F(SingleFileAnimatorTest, shouldSetIdleAnimationType)
{
    singleFileAnimator.setAnimation(AnimationType::Idle);
    ASSERT_EQ(singleFileAnimator.getAnimationType(), AnimationType::Idle);
}

TEST_F(SingleFileAnimatorTest, shouldSetAnimationDirection)
{
    singleFileAnimator.setAnimationDirection(AnimationDirection::Left);

    ASSERT_EQ(singleFileAnimator.getAnimationDirection(), AnimationDirection::Left);
}

TEST_F(SingleFileAnimatorTest, givenTimeNotExceedingTimeBetweenTextures_shouldNotUpdateAnimation)
{
    const auto animationChanged = singleFileAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_FALSE(animationChanged);
}

TEST_F(SingleFileAnimatorTest, givenTimeExceedingTimeBetweenTextures_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTextureRect, scaleRightDirection));

    const auto animationChanged = singleFileAnimator.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(SingleFileAnimatorTest,
       givenTimeExceedingTimeBetweenTexturesWithLeftDirectedAnimation_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId2, firstIdleTextureRect, scaleLeftDirection));

    const auto animationChanged =
        singleFileAnimatorWithLeftInitialDirection.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(SingleFileAnimatorTest,
       givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationTypeSet_shouldUpdateAnimation)
{
    singleFileAnimator.setAnimation(AnimationType::Walk);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstWalkTextureRect, scaleRightDirection));

    const auto animationChanged = singleFileAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(SingleFileAnimatorTest,
       givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationDirectionSet_shouldUpdateAnimation)
{
    singleFileAnimator.setAnimationDirection(AnimationDirection::Left);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTextureRect, scaleLeftDirection));

    const auto animationChanged = singleFileAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(SingleFileAnimatorTest, givenNotSupportedAnimationType_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(singleFileAnimator.setAnimation(notSupportedAnimationType),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(SingleFileAnimatorTest, givenSupportedAnimationType_shouldNotThrowAndChangeAnimationType)
{
    ASSERT_NO_THROW(singleFileAnimator.setAnimation(supportedAnimationType));
    ASSERT_EQ(singleFileAnimator.getAnimationType(), supportedAnimationType);
}

TEST_F(SingleFileAnimatorTest, givenNotSupportedAnimationTypeAndDirection_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(singleFileAnimator.setAnimation(notSupportedAnimationType, AnimationDirection::Right),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(SingleFileAnimatorTest,
       givenSupportedAnimationTypeAndDirection_shouldNotThrowAndChangeAnimationTypeAndDirection)
{
    ASSERT_NO_THROW(singleFileAnimator.setAnimation(supportedAnimationType, AnimationDirection::Right));

    ASSERT_EQ(singleFileAnimator.getAnimationType(), supportedAnimationType);
    ASSERT_EQ(singleFileAnimator.getAnimationDirection(), AnimationDirection::Right);
}

TEST_F(SingleFileAnimatorTest, getCurrentAnimationProgress)
{
    singleFileAnimator.setAnimation(AnimationType::Walk);

    ASSERT_EQ(singleFileAnimator.getCurrentAnimationProgressInPercents(), 25);
}
