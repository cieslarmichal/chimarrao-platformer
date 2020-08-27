#include "AnimationsFromSettingsLoader.h"

#include "gtest/gtest.h"

using namespace ::testing;
using namespace graphics::animation;

class AnimationsFromSettingsLoaderTest : public Test
{
public:
    const std::vector<std::string> textures{"1", "2", "3"};
    const Animation animation{textures, 60};
    std::unordered_map<AnimationType, Animation> nonEmptyAnimations{{AnimationType::Jump, animation}};
    std::unordered_map<AnimationType, Animation> animations;
    const AnimationsSettings emptyAnimationsSettings{};
    const AnimationsSettings animationsSettings{{"idle", "/dev/x1.txt", 3, 0.4},
                                                {"walk", "/elo/123.txt", 2, 0.7}};
};

TEST_F(AnimationsFromSettingsLoaderTest, givenNonEmptyAnimationsAndEmptySettings_ShouldClearAnimations)
{
    AnimationsFromSettingsLoader::loadAnimationsFromSettings(nonEmptyAnimations, emptyAnimationsSettings);

    ASSERT_TRUE(nonEmptyAnimations.empty());
}

TEST_F(AnimationsFromSettingsLoaderTest, givenEmptyAnimationsAndEmptySettings_ShouldNotChangeAnimations)
{
    AnimationsFromSettingsLoader::loadAnimationsFromSettings(animations, emptyAnimationsSettings);

    ASSERT_TRUE(animations.empty());
}

TEST_F(AnimationsFromSettingsLoaderTest, shouldLoadAnimationsFromSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromSettings(animations, animationsSettings);

    ASSERT_EQ(animations.at(AnimationType::Idle).getCurrentTexturePath(), "/dev/x1.txt");
    ASSERT_EQ(animations.at(AnimationType::Walk).getCurrentTexturePath(), "/elo/123.txt");
}
