#include "MultipleFileAnimator.h"

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

class MultipleFileAnimatorTest_Base : public Test
{
public:
    MultipleFileAnimatorTest_Base()
    {
        expectAnimatorsSettingFirstTextureWithCreation();

        EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings(animatorName))
            .WillRepeatedly(Return(animatorSettings));
    }

    void expectAnimatorsSettingFirstTextureWithCreation()
    {
        EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTextureRect, scaleRightDirection));
        EXPECT_CALL(*rendererPool, setTexture(graphicsId2, firstIdleTextureRect, scaleRightDirection));
    }

    const std::string animatorName{"druid"};
    const GraphicsId graphicsId1{GraphicsIdGenerator::generateId()};
    const GraphicsId graphicsId2{GraphicsIdGenerator::generateId()};
    const std::vector<MultipleFilesAnimationSettings> emptyAnimationsSettings{};
    const utils::DeltaTime timeNotExceedingTimeBetweenTextures{1.0};
    const utils::DeltaTime timeExceedingTimeBetweenTextures{2.5};
    const utils::Vector2f scaleRightDirection{1.0f, 1.0f};
    const utils::Vector2f scaleLeftDirection{-1.0f, 1.0f};
    const std::vector<MultipleFilesAnimationSettings> animationsSettings{{"idle", "idle/x1.txt", 3, 1.2f},
                                                                         {"walk", "walk/123.txt", 2, 2.0f}};
    const std::string projectPath{utils::ProjectPathReader::getProjectRootPath()};
    const TextureRect firstIdleTextureRect{projectPath + "idle/x1.txt"};
    const TextureRect secondIdleTextureRect{projectPath + "idle/x2.txt"};
    const TextureRect firstWalkTextureRect{projectPath + "walk/123.txt"};
    const MultipleFilesAnimatorSettings animatorSettingsWithDifferentName{"diffName", animationsSettings};
    const MultipleFilesAnimatorSettings animatorSettingsWithEmptyAnimationsSettings{animatorName,
                                                                                    emptyAnimationsSettings};
    const MultipleFilesAnimatorSettings animatorSettings{animatorName, animationsSettings};
    const AnimationType notSupportedAnimationType{AnimationType::Jump};
    const AnimationType supportedAnimationType{AnimationType::Walk};
    std::shared_ptr<RendererPoolMock> rendererPool = std::make_shared<StrictMock<RendererPoolMock>>();
    std::shared_ptr<StrictMock<AnimatorSettingsRepositoryMock>> animatorSettingsRepo =
        std::make_shared<StrictMock<AnimatorSettingsRepositoryMock>>();
};

class MultipleFileAnimatorTest : public MultipleFileAnimatorTest_Base
{
public:
    MultipleFileAnimator multipleFileAnimator{graphicsId1, rendererPool, animatorSettingsRepo, animatorName,
                                              AnimationType::Idle};
    MultipleFileAnimator multipleFileAnimatorWithLeftInitialDirection{
        graphicsId2,  rendererPool,        animatorSettingsRepo,
        animatorName, AnimationType::Idle, AnimationDirection::Left};
};

TEST_F(MultipleFileAnimatorTest, givenNoneAnimatorSettings_shouldThrowInvalidAnimatorSettingsNotFound)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings(animatorName))
        .WillOnce(Return(boost::none));

    ASSERT_THROW(MultipleFileAnimator(graphicsId1, rendererPool, animatorSettingsRepo, animatorName,
                                      AnimationType::Idle),
                 animations::exceptions::AnimatorSettingsNotFound);
}

TEST_F(MultipleFileAnimatorTest,
       givenAnimatorSettingsWithDifferentNameThanAnimatorName_shouldThrowInvalidAnimatorConfigFile)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings(animatorName))
        .WillOnce(Return(animatorSettingsWithDifferentName));

    ASSERT_THROW(MultipleFileAnimator(graphicsId1, rendererPool, animatorSettingsRepo, animatorName,
                                      AnimationType::Idle),
                 animations::exceptions::InvalidAnimatorSettings);
}

TEST_F(MultipleFileAnimatorTest,
       givenInitialAnimationTypeDifferentThanAnimatorAnimationsType_shouldThrowAnimationTypeNotSupported)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings(animatorName))
        .WillOnce(Return(animatorSettings));

    ASSERT_THROW(MultipleFileAnimator(graphicsId1, rendererPool, animatorSettingsRepo, animatorName,
                                      AnimationType::Jump),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(MultipleFileAnimatorTest,
       givenAnimatorSettingsWithEmptyAnimations_shouldThrowAnimationsFromSettingsNotFound)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings(animatorName))
        .WillOnce(Return(animatorSettingsWithEmptyAnimationsSettings));

    ASSERT_THROW(MultipleFileAnimator(graphicsId1, rendererPool, animatorSettingsRepo, animatorName,
                                      AnimationType::Idle),
                 animations::exceptions::AnimationsFromSettingsNotFound);
}

TEST_F(MultipleFileAnimatorTest, getInitialAnimationType_shouldReturnIdle)
{
    ASSERT_EQ(multipleFileAnimator.getAnimationType(), AnimationType::Idle);
}

TEST_F(MultipleFileAnimatorTest, getInitialDefaultAnimationDirection_shouldReturnAnimationDirectionRight)
{
    ASSERT_EQ(multipleFileAnimator.getAnimationDirection(), AnimationDirection::Right);
}

TEST_F(MultipleFileAnimatorTest, shouldSetAnimationType)
{
    multipleFileAnimator.setAnimation(AnimationType::Walk);

    ASSERT_EQ(multipleFileAnimator.getAnimationType(), AnimationType::Walk);
}

TEST_F(MultipleFileAnimatorTest, shouldSetAnimationDirection)
{
    multipleFileAnimator.setAnimationDirection(AnimationDirection::Left);

    ASSERT_EQ(multipleFileAnimator.getAnimationDirection(), AnimationDirection::Left);
}

TEST_F(MultipleFileAnimatorTest, givenTimeNotExceedingTimeBetweenTextures_shouldNotUpdateAnimation)
{
    const auto animationChanged = multipleFileAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_FALSE(animationChanged);
}

TEST_F(MultipleFileAnimatorTest, givenTimeExceedingTimeBetweenTextures_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, secondIdleTextureRect, scaleRightDirection));

    const auto animationChanged = multipleFileAnimator.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(MultipleFileAnimatorTest,
       givenTimeExceedingTimeBetweenTexturesWithLeftDirectedAnimation_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId2, secondIdleTextureRect, scaleLeftDirection));

    const auto animationChanged =
        multipleFileAnimatorWithLeftInitialDirection.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(MultipleFileAnimatorTest,
       givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationTypeSet_shouldUpdateAnimation)
{
    multipleFileAnimator.setAnimation(AnimationType::Walk);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstWalkTextureRect, scaleRightDirection));

    const auto animationChanged = multipleFileAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(MultipleFileAnimatorTest,
       givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationDirectionSet_shouldUpdateAnimation)
{
    multipleFileAnimator.setAnimationDirection(AnimationDirection::Left);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTextureRect, scaleLeftDirection));

    const auto animationChanged = multipleFileAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(MultipleFileAnimatorTest, givenNotSupportedAnimationType_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(multipleFileAnimator.setAnimation(notSupportedAnimationType),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(MultipleFileAnimatorTest, givenSupportedAnimationType_shouldNotThrowAndChangeAnimationType)
{
    ASSERT_NO_THROW(multipleFileAnimator.setAnimation(supportedAnimationType));
    ASSERT_EQ(multipleFileAnimator.getAnimationType(), supportedAnimationType);
}

TEST_F(MultipleFileAnimatorTest, givenNotSupportedAnimationTypeAndDirection_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(multipleFileAnimator.setAnimation(notSupportedAnimationType, AnimationDirection::Right),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(MultipleFileAnimatorTest,
       givenSupportedAnimationTypeAndDirection_shouldNotThrowAndChangeAnimationTypeAndDirection)
{
    ASSERT_NO_THROW(multipleFileAnimator.setAnimation(supportedAnimationType, AnimationDirection::Right));

    ASSERT_EQ(multipleFileAnimator.getAnimationType(), supportedAnimationType);
    ASSERT_EQ(multipleFileAnimator.getAnimationDirection(), AnimationDirection::Right);
}
