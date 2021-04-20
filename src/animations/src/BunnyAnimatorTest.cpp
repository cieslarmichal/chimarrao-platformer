#include "BunnyAnimator.h"

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

class BunnyAnimatorTest_Base : public Test
{
public:
    BunnyAnimatorTest_Base()
    {
        expectAnimatorsSettingFirstTextureWithCreation();

        EXPECT_CALL(*animatorSettingsRepo, getSingleFileAnimatorSettings("bunny"))
            .WillRepeatedly(Return(animatorSettings));
    }

    void expectAnimatorsSettingFirstTextureWithCreation()
    {
        EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTextureRect, scaleRightDirection));
        EXPECT_CALL(*rendererPool, setTexture(graphicsId2, firstIdleTextureRect, scaleRightDirection));
    }

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
    const SingleFileAnimatorSettings animatorSettingsWithEmptyAnimations{"bunny", {}};
    const AnimationType notSupportedAnimationType{AnimationType::Jump};
    const AnimationType supportedAnimationType{AnimationType::Walk};
    std::shared_ptr<RendererPoolMock> rendererPool = std::make_shared<StrictMock<RendererPoolMock>>();
    std::shared_ptr<StrictMock<AnimatorSettingsRepositoryMock>> animatorSettingsRepo =
        std::make_shared<StrictMock<AnimatorSettingsRepositoryMock>>();
};

class BunnyAnimatorTest : public BunnyAnimatorTest_Base
{
public:
    BunnyAnimator bunnyAnimator{graphicsId1, rendererPool, animatorSettingsRepo, AnimationType::Idle};
    BunnyAnimator bunnyAnimatorWithLeftInitialDirection{graphicsId2, rendererPool, animatorSettingsRepo,
                                                        AnimationType::Idle, AnimationDirection::Left};
};

TEST_F(BunnyAnimatorTest, givenNoneAnimatorSettings_shouldThrowInvalidAnimatorSettingsNotFound)
{
    EXPECT_CALL(*animatorSettingsRepo, getSingleFileAnimatorSettings("bunny")).WillOnce(Return(boost::none));

    ASSERT_THROW(BunnyAnimator(graphicsId1, rendererPool, animatorSettingsRepo),
                 animations::exceptions::AnimatorSettingsNotFound);
}

TEST_F(BunnyAnimatorTest,
       givenAnimatorSettingsWithDifferentNameThanPlayer_shouldThrowInvalidAnimatorConfigFile)
{
    EXPECT_CALL(*animatorSettingsRepo, getSingleFileAnimatorSettings("bunny"))
        .WillOnce(Return(animatorSettingsWithDifferentAnimatorName));

    ASSERT_THROW(BunnyAnimator(graphicsId1, rendererPool, animatorSettingsRepo),
                 animations::exceptions::InvalidAnimatorSettings);
}

TEST_F(BunnyAnimatorTest,
       givenInitialAnimationTypeDifferentThanPlayersAnimationsType_shouldThrowAnimationTypeNotSupported)
{
    EXPECT_CALL(*animatorSettingsRepo, getSingleFileAnimatorSettings("bunny"))
        .WillOnce(Return(animatorSettings));

    ASSERT_THROW(BunnyAnimator(graphicsId1, rendererPool, animatorSettingsRepo, AnimationType::Jump),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(BunnyAnimatorTest, givenAnimatorSettingsWithEmptyAnimations_shouldThrowAnimationsFromSettingsNotFound)
{
    EXPECT_CALL(*animatorSettingsRepo, getSingleFileAnimatorSettings("bunny"))
        .WillOnce(Return(animatorSettingsWithEmptyAnimations));

    ASSERT_THROW(BunnyAnimator(graphicsId1, rendererPool, animatorSettingsRepo),
                 animations::exceptions::AnimationsFromSettingsNotFound);
}

TEST_F(BunnyAnimatorTest, getInitialAnimationType_shouldReturnIdle)
{
    ASSERT_EQ(bunnyAnimator.getAnimationType(), AnimationType::Idle);
}

TEST_F(BunnyAnimatorTest, getInitialDefaultAnimationDirection_shouldReturnAnimationDirectionRight)
{
    ASSERT_EQ(bunnyAnimator.getAnimationDirection(), AnimationDirection::Right);
}

TEST_F(BunnyAnimatorTest, shouldSetWalkAnimationType)
{
    bunnyAnimator.setAnimation(AnimationType::Walk);
    ASSERT_EQ(bunnyAnimator.getAnimationType(), AnimationType::Walk);
}

TEST_F(BunnyAnimatorTest, shouldSetIdleAnimationType)
{
    bunnyAnimator.setAnimation(AnimationType::Idle);
    ASSERT_EQ(bunnyAnimator.getAnimationType(), AnimationType::Idle);
}

TEST_F(BunnyAnimatorTest, shouldSetAnimationDirection)
{
    bunnyAnimator.setAnimationDirection(AnimationDirection::Left);

    ASSERT_EQ(bunnyAnimator.getAnimationDirection(), AnimationDirection::Left);
}

TEST_F(BunnyAnimatorTest, givenTimeNotExceedingTimeBetweenTextures_shouldNotUpdateAnimation)
{
    const auto animationChanged = bunnyAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_FALSE(animationChanged);
}

TEST_F(BunnyAnimatorTest, givenTimeExceedingTimeBetweenTextures_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTextureRect, scaleRightDirection));

    const auto animationChanged = bunnyAnimator.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(BunnyAnimatorTest,
       givenTimeExceedingTimeBetweenTexturesWithLeftDirectedAnimation_shouldUpdateAnimation)
{
    EXPECT_CALL(*rendererPool, setTexture(graphicsId2, firstIdleTextureRect, scaleLeftDirection));

    const auto animationChanged =
        bunnyAnimatorWithLeftInitialDirection.update(timeExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(BunnyAnimatorTest,
       givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationTypeSet_shouldUpdateAnimation)
{
    bunnyAnimator.setAnimation(AnimationType::Walk);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstWalkTextureRect, scaleRightDirection));

    const auto animationChanged = bunnyAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(BunnyAnimatorTest,
       givenTimeNotExceedingTimeBetweenTextures_andGivenAnimationDirectionSet_shouldUpdateAnimation)
{
    bunnyAnimator.setAnimationDirection(AnimationDirection::Left);
    EXPECT_CALL(*rendererPool, setTexture(graphicsId1, firstIdleTextureRect, scaleLeftDirection));

    const auto animationChanged = bunnyAnimator.update(timeNotExceedingTimeBetweenTextures);

    ASSERT_TRUE(animationChanged);
}

TEST_F(BunnyAnimatorTest, givenNotSupportedAnimationType_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(bunnyAnimator.setAnimation(notSupportedAnimationType),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(BunnyAnimatorTest, givenSupportedAnimationType_shouldNotThrowAndChangeAnimationType)
{
    ASSERT_NO_THROW(bunnyAnimator.setAnimation(supportedAnimationType));
    ASSERT_EQ(bunnyAnimator.getAnimationType(), supportedAnimationType);
}

TEST_F(BunnyAnimatorTest, givenNotSupportedAnimationTypeAndDirection_shouldThrowAnimationNotSupported)
{
    ASSERT_THROW(bunnyAnimator.setAnimation(notSupportedAnimationType, AnimationDirection::Right),
                 animations::exceptions::AnimationTypeNotSupported);
}

TEST_F(BunnyAnimatorTest,
       givenSupportedAnimationTypeAndDirection_shouldNotThrowAndChangeAnimationTypeAndDirection)
{
    ASSERT_NO_THROW(bunnyAnimator.setAnimation(supportedAnimationType, AnimationDirection::Right));

    ASSERT_EQ(bunnyAnimator.getAnimationType(), supportedAnimationType);
    ASSERT_EQ(bunnyAnimator.getAnimationDirection(), AnimationDirection::Right);
}
