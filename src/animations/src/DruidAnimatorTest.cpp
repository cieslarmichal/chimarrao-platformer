#include "DruidAnimator.h"

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

class DruidAnimatorTest_Base : public Test
{
public:
    DruidAnimatorTest_Base()
    {
        expectAnimatorsSettingFirstTextureWithCreation();

        EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings("druid"))
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
    const std::string projectPath{utils::ProjectPathReader::getProjectRootPath()};
    const TextureRect firstIdleTextureRect{projectPath + "idle/x1.txt"};
    const TextureRect secondIdleTextureRect{projectPath + "idle/x2.txt"};
    const TextureRect firstWalkTextureRect{projectPath + "walk/123.txt"};
    const MultipleFilesAnimatorSettings animatorSettingsWithDifferentName{"diffName", animationsSettings};
    const MultipleFilesAnimatorSettings animatorSettingsWithEmptyAnimationsSettings{"druid",
                                                                                    emptyAnimationsSettings};
    const MultipleFilesAnimatorSettings animatorSettings{"druid", animationsSettings};
    const AnimationType notSupportedAnimationType{AnimationType::Jump};
    const AnimationType supportedAnimationType{AnimationType::Walk};
    std::shared_ptr<RendererPoolMock> rendererPool = std::make_shared<StrictMock<RendererPoolMock>>();
    std::shared_ptr<StrictMock<AnimatorSettingsRepositoryMock>> animatorSettingsRepo =
        std::make_shared<StrictMock<AnimatorSettingsRepositoryMock>>();
};

class DruidAnimatorTest : public DruidAnimatorTest_Base
{
public:
    DruidAnimator druidAnimator{graphicsId1, rendererPool, animatorSettingsRepo, AnimationType::Idle};
    DruidAnimator druidAnimatorWithLeftInitialDirection{graphicsId2, rendererPool, animatorSettingsRepo,
                                                          AnimationType::Idle, AnimationDirection::Left};
};

TEST_F(DruidAnimatorTest, givenNoneAnimatorSettings_shouldThrowInvalidAnimatorSettingsNotFound)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings("druid"))
        .WillOnce(Return(boost::none));

    ASSERT_THROW(DruidAnimator(graphicsId1, rendererPool, animatorSettingsRepo),
                 animations::exceptions::AnimatorSettingsNotFound);
}

TEST_F(DruidAnimatorTest,
       givenAnimatorSettingsWithDifferentNameThanPlayer_shouldThrowInvalidAnimatorConfigFile)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings("druid"))
        .WillOnce(Return(animatorSettingsWithDifferentName));

    ASSERT_THROW(DruidAnimator(graphicsId1, rendererPool, animatorSettingsRepo),
                 animations::exceptions::InvalidAnimatorSettings);
}

TEST_F(DruidAnimatorTest,
       givenInitialAnimationTypeDifferentThanPlayersAnimationsType_shouldThrowAnimationTypeNotSupported)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings("druid"))
        .WillOnce(Return(animatorSettings));

    ASSERT_THROW(DruidAnimator(graphicsId1, rendererPool, animatorSettingsRepo, AnimationType::Jump),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(DruidAnimatorTest, givenAnimatorSettingsWithEmptyAnimations_shouldThrowAnimationsFromSettingsNotFound)
{
    EXPECT_CALL(*animatorSettingsRepo, getMultipleFileAnimatorSettings("druid"))
        .WillOnce(Return(animatorSettingsWithEmptyAnimationsSettings));

    ASSERT_THROW(DruidAnimator(graphicsId1, rendererPool, animatorSettingsRepo),
                 animations::exceptions::AnimationsFromSettingsNotFound);
}

TEST_F(DruidAnimatorTest, getInitialAnimationType_shouldReturnIdle)
{
    ASSERT_EQ(druidAnimator.getAnimationType(), AnimationType::Idle);
}

TEST_F(DruidAnimatorTest, getInitialDefaultAnimationDirection_shouldReturnAnimationDirectionRight)
{
    ASSERT_EQ(druidAnimator.getAnimationDirection(), AnimationDirection::Right);
}

TEST_F(DruidAnimatorTest, shouldSetAnimationType)
{
    druidAnimator.setAnimation(AnimationType::Walk);

    ASSERT_EQ(druidAnimator.getAnimationType(), AnimationType::Walk);
}

TEST_F(DruidAnimatorTest, shouldSetAnimationDirection)
{
    druidAnimator.setAnimationDirection(AnimationDirection::Left);

    ASSERT_EQ(druidAnimator.getAnimationDirection(), AnimationDirection::Left);
}

TEST_F(DruidAnimatorTest, givenTimeNotExceedingTimeBetweenTextures_shouldNotUpdateAnimation)
{
    const auto animationChanged = druidAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_FALSE(animationChanged);
}

TEST_F(DruidAnimatorTest, givenTimeExceedingTimeBetweenTextures_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, secondIdleTextureRect, scaleRightDirection));

    const auto animationChanged = druidAnimator.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(DruidAnimatorTest,
       givenTimeExceedingTimeBetweenTexturesWithLeftDirectedAnimation_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId2, secondIdleTextureRect, scaleLeftDirection));

    const auto animationChanged =
        druidAnimatorWithLeftInitialDirection.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(DruidAnimatorTest,
       givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationTypeSet_shouldUpdateAnimation)
{
    druidAnimator.setAnimation(AnimationType::Walk);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstWalkTextureRect, scaleRightDirection));

    const auto animationChanged = druidAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(DruidAnimatorTest,
       givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationDirectionSet_shouldUpdateAnimation)
{
    druidAnimator.setAnimationDirection(AnimationDirection::Left);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTextureRect, scaleLeftDirection));

    const auto animationChanged = druidAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(DruidAnimatorTest, givenNotSupportedAnimationType_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(druidAnimator.setAnimation(notSupportedAnimationType),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(DruidAnimatorTest, givenSupportedAnimationType_shouldNotThrowAndChangeAnimationType)
{
    ASSERT_NO_THROW(druidAnimator.setAnimation(supportedAnimationType));
    ASSERT_EQ(druidAnimator.getAnimationType(), supportedAnimationType);
}

TEST_F(DruidAnimatorTest, givenNotSupportedAnimationTypeAndDirection_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(druidAnimator.setAnimation(notSupportedAnimationType, AnimationDirection::Right),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(DruidAnimatorTest,
       givenSupportedAnimationTypeAndDirection_shouldNotThrowAndChangeAnimationTypeAndDirection)
{
    ASSERT_NO_THROW(druidAnimator.setAnimation(supportedAnimationType, AnimationDirection::Right));

    ASSERT_EQ(druidAnimator.getAnimationType(), supportedAnimationType);
    ASSERT_EQ(druidAnimator.getAnimationDirection(), AnimationDirection::Right);
}
