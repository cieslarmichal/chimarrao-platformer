#include "AnimationsFromSettingsLoader.h"

#include "gtest/gtest.h"

#include "GetProjectPath.h"
#include "exceptions/AnimationsFromSettingsNotFound.h"

using namespace ::testing;
using namespace animations;

class AnimationsFromSettingsLoaderTest : public Test
{
public:
    std::vector<graphics::TextureRect> textureRects{{"1"}, {"2"}, {"3"}};
    const Animation animation{textureRects, 60};
    std::unordered_map<AnimationType, Animation> nonEmptyAnimations{{AnimationType::Jump, animation}};
    std::unordered_map<AnimationType, Animation> animations;
    const std::vector<MultipleFilesAnimationSettings> emptyAnimationsSettings{};
    const std::string projectPath{utils::getProjectPath("chimarrao-platformer")};
    const std::vector<MultipleFilesAnimationSettings> animationsSettings{{"idle", "dev/x1.txt", 3, 0.4f},
                                                                         {"walk", "elo/123.txt", 2, 0.7f}};
};

TEST_F(AnimationsFromSettingsLoaderTest, givenEmptyAnimationsSettings_shouldThrowAnimationsNotFound)
{
    ASSERT_THROW(AnimationsFromSettingsLoader::loadAnimationsFromMultipleFilesAnimationsSettings(
                     animations, emptyAnimationsSettings),
                 exceptions::AnimationsFromSettingsNotFound);
}

TEST_F(AnimationsFromSettingsLoaderTest, shouldLoadAnimationsFromSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromMultipleFilesAnimationsSettings(animations,
                                                                                    animationsSettings);

    ASSERT_EQ(animations.at(AnimationType::Idle).getCurrentTextureRect(),
              graphics::TextureRect{projectPath + "dev/x1.txt"});
    ASSERT_EQ(animations.at(AnimationType::Walk).getCurrentTextureRect(),
              graphics::TextureRect{projectPath + "elo/123.txt"});
}

TEST_F(AnimationsFromSettingsLoaderTest,
       givenNonEmptyAnimations_shouldClearAnimationsAndLoadAnimationsFromSettings)
{
    AnimationsFromSettingsLoader::loadAnimationsFromMultipleFilesAnimationsSettings(nonEmptyAnimations,
                                                                                    animationsSettings);

    ASSERT_TRUE(nonEmptyAnimations.count(AnimationType::Jump) == 0);
    ASSERT_EQ(nonEmptyAnimations.at(AnimationType::Idle).getCurrentTextureRect(),
              graphics::TextureRect{projectPath + "dev/x1.txt"});
    ASSERT_EQ(nonEmptyAnimations.at(AnimationType::Walk).getCurrentTextureRect(),
              graphics::TextureRect{projectPath + "elo/123.txt"});
}
