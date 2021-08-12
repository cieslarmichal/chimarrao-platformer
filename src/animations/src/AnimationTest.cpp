#include "Animation.h"

#include "gtest/gtest.h"

#include "exceptions/TexturesNotInitialized.h"

using namespace ::testing;
using namespace animations;

class AnimationTest : public Test
{
public:
    std::vector<graphics::TextureRect> textureRects{{"1"}, {"2"}, {"3"}};
    std::vector<graphics::TextureRect> emptyTextureRects{};
    const float timeBetweenTextures = 60;
};

TEST_F(AnimationTest, creationOfAnimationWithEmptyTextures_shouldThrowTexturesNotInitialized)
{
    ASSERT_THROW(Animation(emptyTextureRects, timeBetweenTextures, false, false),
                 exceptions::TexturesNotInitialized);
}

TEST_F(AnimationTest, newAnimationShouldReturnFirstFrame)
{
    Animation animation{textureRects, timeBetweenTextures, true, false};
    ASSERT_EQ(textureRects[0], animation.getCurrentTextureRect());
}

TEST_F(AnimationTest, aniationShouldReturnNextTexture)
{
    Animation animation{textureRects, timeBetweenTextures, true, false};
    const auto animationChanged = animation.update(utils::DeltaTime(timeBetweenTextures + 1));

    ASSERT_TRUE(animationChanged);
    ASSERT_EQ(textureRects[1], animation.getCurrentTextureRect());
}

TEST_F(AnimationTest, animationShouldNotReturnNextTexture)
{
    Animation animation{textureRects, timeBetweenTextures, true, false};
    const auto animationChanged = animation.update(utils::DeltaTime(timeBetweenTextures - 1));

    ASSERT_FALSE(animationChanged);
    ASSERT_EQ(textureRects[0], animation.getCurrentTextureRect());
}

TEST_F(AnimationTest, givenAnimationWithLoopsAllowed_animationShouldReturnFirstTextureAfterLast)
{
    Animation animation{textureRects, timeBetweenTextures, true, false};

    animation.update(utils::DeltaTime(timeBetweenTextures + 1));
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));

    ASSERT_EQ(textureRects[0], animation.getCurrentTextureRect());
}

TEST_F(AnimationTest, givenAnimationWithLoopsNotAllowed_animationShouldStayAtLastTexture)
{
    Animation animation{textureRects, timeBetweenTextures, false, false};

    animation.update(utils::DeltaTime(timeBetweenTextures + 1));
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));

    ASSERT_EQ(textureRects[2], animation.getCurrentTextureRect());
}

TEST_F(AnimationTest, givenAnimationWithLoopsNotAllowed_andLastTexture_shouldReturnAnimationFinished)
{
    Animation animation{textureRects, timeBetweenTextures, false, false};

    animation.update(utils::DeltaTime(timeBetweenTextures + 1));
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));

    ASSERT_TRUE(animation.hasAnimationFinished());
}

TEST_F(AnimationTest, givenAnimationWithLoopsAllowed_andLastTexture_shouldReturnAnimationNotFinished)
{
    Animation animation{textureRects, timeBetweenTextures, true, false};

    animation.update(utils::DeltaTime(timeBetweenTextures + 1));
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));

    ASSERT_FALSE(animation.hasAnimationFinished());
}

TEST_F(AnimationTest, givenAnimationWithLoopsNotAllowed_andFirstTexture_shouldReturnAnimationNotFinished)
{
    Animation animation{textureRects, timeBetweenTextures, false, false};

    ASSERT_FALSE(animation.hasAnimationFinished());
}

TEST_F(AnimationTest, givenAnimationWithLoopsAllowed_andFirstTexture_shouldReturnAnimationNotFinished)
{
    Animation animation{textureRects, timeBetweenTextures, true, false};

    ASSERT_FALSE(animation.hasAnimationFinished());
}

TEST_F(AnimationTest, areInterruptionsAllowed)
{
    Animation animationWithInterruptionsAllowed{textureRects, timeBetweenTextures, true, true};
    Animation animationWithInterruptionsNotAllowed{textureRects, timeBetweenTextures, true, false};

    ASSERT_TRUE(animationWithInterruptionsAllowed.areInterruptionsAllowed());
    ASSERT_FALSE(animationWithInterruptionsNotAllowed.areInterruptionsAllowed());
}

TEST_F(AnimationTest, givenFirstTexture_shouldReturnProgressAs33Percents)
{
    Animation animation{textureRects, timeBetweenTextures, false, false};

    const auto actualAnimationProgress = animation.getProgressInPercents();

    ASSERT_EQ(actualAnimationProgress, 33);
}

TEST_F(AnimationTest, givenSecondTexture_shouldReturnProgressAs66Percents)
{
    Animation animation{textureRects, timeBetweenTextures, false, false};
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));

    const auto actualAnimationProgress = animation.getProgressInPercents();

    ASSERT_EQ(actualAnimationProgress, 66);
}

TEST_F(AnimationTest, givenSecondTexture_shouldReturnProgressAs100Percents)
{
    Animation animation{textureRects, timeBetweenTextures, false, false};
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));

    const auto actualAnimationProgress = animation.getProgressInPercents();

    ASSERT_EQ(actualAnimationProgress, 100);
}
