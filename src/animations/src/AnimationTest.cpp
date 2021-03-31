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
    float timeBetweenTextures = 60;
    Animation animation{textureRects, timeBetweenTextures};
};

TEST_F(AnimationTest, creationOfAnimationWithEmptyTextures_shouldThrowTexturesNotInitialized)
{
    ASSERT_THROW(Animation(emptyTextureRects, timeBetweenTextures), exceptions::TexturesNotInitialized);
}

TEST_F(AnimationTest, newAnimationShouldReturnFirstFrame)
{
    ASSERT_EQ(textureRects[0], animation.getCurrentTextureRect());
}

TEST_F(AnimationTest, aniationShouldReturnNextTexture)
{
    const auto animationChanged = animation.update(utils::DeltaTime(timeBetweenTextures + 1));

    ASSERT_TRUE(animationChanged);
    ASSERT_EQ(textureRects[1], animation.getCurrentTextureRect());
}

TEST_F(AnimationTest, animationShouldNotReturnNextTexture)
{
    const auto animationChanged = animation.update(utils::DeltaTime(timeBetweenTextures - 1));

    ASSERT_FALSE(animationChanged);
    ASSERT_EQ(textureRects[0], animation.getCurrentTextureRect());
}

TEST_F(AnimationTest, animationShouldReturnFirstTextureAfterLast)
{
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));
    animation.update(utils::DeltaTime(timeBetweenTextures + 1));

    ASSERT_EQ(textureRects[0], animation.getCurrentTextureRect());
}
