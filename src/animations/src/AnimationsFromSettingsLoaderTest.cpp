#include "AnimationsFromSettingsLoader.h"

#include "gtest/gtest.h"

#include "GetProjectPath.h"
#include "exceptions/AnimationsFromSettingsNotFound.h"

using namespace ::testing;
using namespace animations;

class AnimationsFromSettingsLoaderTest : public Test
{
public:
    const std::vector<std::string> textures{"1", "2", "3"};
    const Animation animation{textures, 60};
    std::unordered_map<AnimationType, Animation> nonEmptyAnimations{{AnimationType::Jump, animation}};
    std::unordered_map<AnimationType, Animation> animations;
    const AnimationsSettings emptyAnimationsSettings{};
    const std::string projectPath{utils::getProjectPath("chimarrao-platformer")};
    const AnimationsSettings animationsSettings{{"idle", "dev/x1.txt", 3, 0.4},
                                                {"walk", "elo/123.txt", 2, 0.7}};
};

TEST_F(AnimationsFromSettingsLoaderTest, givenEmptyAnimationsSettings_shouldThrowAnimationsNotFound)
{
    ASSERT_THROW(
        AnimationsFromSettingsLoader::loadAnimationsFromSettings(animations, emptyAnimationsSettings),
        exceptions::AnimationsFromSettingsNotFound);
}

TEST_F(AnimationsFromSettingsLoaderTest, shouldLoadAnimationsFromSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromSettings(animations, animationsSettings);

    ASSERT_EQ(animations.at(AnimationType::Idle).getCurrentTexturePath(), projectPath + "dev/x1.txt");
    ASSERT_EQ(animations.at(AnimationType::Walk).getCurrentTexturePath(), projectPath + "elo/123.txt");
}

TEST_F(AnimationsFromSettingsLoaderTest,
       givenNonEmptyAnimations_shouldClearAnimationsAndLoadAnimationsFromSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromSettings(nonEmptyAnimations, animationsSettings);

    ASSERT_TRUE(nonEmptyAnimations.count(AnimationType::Jump) == 0);
    ASSERT_EQ(nonEmptyAnimations.at(AnimationType::Idle).getCurrentTexturePath(), projectPath + "dev/x1.txt");
    ASSERT_EQ(nonEmptyAnimations.at(AnimationType::Walk).getCurrentTexturePath(),
              projectPath + "elo/123.txt");
}
