#include "AnimatorSettingsYamlReader.h"

#include "gtest/gtest.h"

#include "ProjectPathReader.h"
#include "exceptions/AnimatorsConfigFileNotFound.h"
#include "exceptions/InvalidAnimatorsConfigFile.h"

using namespace ::testing;
using namespace animations;

namespace
{
const std::string projectPath{utils::ProjectPathReader::getProjectRootPath()};
const std::string testDirectory = projectPath + "src/animations/src/testFiles/";
const std::string invalidPath{testDirectory + "aaaaaaa.yaml"};
const std::string invalidYaml{testDirectory + "invalidYaml.yaml"};
const std::string configWithAnimatorWithoutAnimationsField{testDirectory +
                                                           "configWithAnimatorWithoutAnimationsField.yaml"};
const std::string configWithAnimatorWithoutAnimationTexturePathField{
    testDirectory + "configWithAnimatorWithoutAnimationTexturePathField.yaml"};
const std::string configWithAnimatorWithoutAnimationTypeField{
    testDirectory + "configWithAnimatorWithoutAnimationTypeField.yaml"};
const std::string configWithAnimatorWithoutAnimatorNameField{
    testDirectory + "configWithAnimatorWithoutAnimatorNameField.yaml"};
const std::string configWithAnimatorWithoutNumberOfAnimationTexturesField{
    testDirectory + "configWithAnimatorWithoutNumberOfAnimationTexturesField.yaml"};
const std::string configWithAnimatorWithoutTimeBetweenTexturesField{
    testDirectory + "configWithAnimatorWithoutTimeBetweenTexturesField.yaml"};
const std::string configWithEmptyAnimators{testDirectory + "configWithEmptyAnimators.yaml"};
const std::string configWithoutAnimatorsField{testDirectory + "configWithoutAnimatorsField.yaml"};
const std::string validPathWithOneAnimator{testDirectory + "validConfigWithOneAnimator.yaml"};
const std::string validPathWithTwoAnimators{testDirectory + "validConfigWithTwoAnimators.yaml"};
const std::string validPathWithSingleFileAnimator{testDirectory + "validConfigWithSingleFileAnimator.yaml"};
const std::string validPathWithSingleFileAnimatorWithMissingFields{
    testDirectory + "validConfigWithSingleFileAnimatorWithMissingFields.yaml"};

const MultipleFilesAnimationSettings playerMultipleFilesAnimationSettings1{
    "idle", "resources/Player/Idle/idle-with-weapon-1.png", 6, 0.3f};
const MultipleFilesAnimationSettings playerMultipleFilesAnimationSettings2{
    "walk", "resources/Player/Walk/walk-with-weapon-1.png", 11, 0.7f};
const std::vector<MultipleFilesAnimationSettings> playerMultipleFilesAnimationsSettings{
    playerMultipleFilesAnimationSettings1, playerMultipleFilesAnimationSettings2};
const MultipleFilesAnimationSettings enemyMultipleFilesAnimationSettings1{
    "idle", "resources/Enemy/Idle/idle-with-weapon-1.png", 3, 0.4f};
const MultipleFilesAnimationSettings enemyMultipleFilesAnimationSettings2{
    "walk", "resources/Enemy/Walk/walk-with-weapon-1.png", 2, 0.8f};
const std::vector<MultipleFilesAnimationSettings> enemyMultipleFilesAnimationsSettings{
    enemyMultipleFilesAnimationSettings1, enemyMultipleFilesAnimationSettings2};
const AnimatorsSettings animatorsSettings1{{}, {{"player", playerMultipleFilesAnimationsSettings}}};
const AnimatorsSettings animatorsSettings2{
    {}, {{"player", playerMultipleFilesAnimationsSettings}, {"enemy", enemyMultipleFilesAnimationsSettings}}};
const SingleFileAnimationSettings bunnySingleFileAnimationSettings{"idle",
                                                                   "resources/bunny-hop-spritesheet.png",
                                                                   utils::Vector2u{192u, 128u},
                                                                   utils::IntRect{48, 85, 48, 43},
                                                                   1,
                                                                   0.1f};
const std::vector<SingleFileAnimationSettings> bunnySingleFileAnimationsSettings{
    bunnySingleFileAnimationSettings};
const AnimatorsSettings bunnyAnimatorsSettings{{{"bunny", bunnySingleFileAnimationsSettings}}, {}};
}

class AnimatorSettingsYamlReaderTest : public Test
{
public:
    AnimatorSettingsYamlReader animatorsSettingsReader;
};

TEST_F(AnimatorSettingsYamlReaderTest, givenInvalidPath_shouldThrowAnimatorsConfigFileNotFound)
{
    ASSERT_THROW(animatorsSettingsReader.readAnimatorsSettings(invalidPath),
                 exceptions::AnimatorsConfigFileNotFound);
}

TEST_F(AnimatorSettingsYamlReaderTest, givenInvalidYamlFile_shouldThrowInvalidAnimatorsConfigFile)
{
    ASSERT_THROW(animatorsSettingsReader.readAnimatorsSettings(invalidYaml),
                 exceptions::InvalidAnimatorsConfigFile);
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenConfigFileWithoutAnimatorsField_shouldThrowInvalidAnimatorsConfigFile)
{
    ASSERT_THROW(animatorsSettingsReader.readAnimatorsSettings(configWithoutAnimatorsField),
                 exceptions::InvalidAnimatorsConfigFile);
}

TEST_F(AnimatorSettingsYamlReaderTest, givenConfigFileWithEmptyAnimators_shouldReturnEmptySettings)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(configWithEmptyAnimators);

    ASSERT_TRUE(actualAnimatorsSettings.singleFileAnimatorsSettings.empty());
    ASSERT_TRUE(actualAnimatorsSettings.multipleFilesAnimatorSettings.empty());
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenConfigFileWithAnimatorWithoutAnimatorNameField_shouldThrowInvalidAnimatorsConfigFile)
{
    ASSERT_THROW(animatorsSettingsReader.readAnimatorsSettings(configWithAnimatorWithoutAnimatorNameField),
                 exceptions::InvalidAnimatorsConfigFile);
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenConfigFileWithAnimatorWithoutAnimationsField_shouldThrowInvalidAnimatorsConfigFile)
{
    ASSERT_THROW(animatorsSettingsReader.readAnimatorsSettings(configWithAnimatorWithoutAnimationsField),
                 exceptions::InvalidAnimatorsConfigFile);
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenConfigFileWithAnimationsWithoutAnimationTypeField_shouldThrowInvalidAnimatorsConfigFile)
{
    ASSERT_THROW(animatorsSettingsReader.readAnimatorsSettings(configWithAnimatorWithoutAnimationTypeField),
                 exceptions::InvalidAnimatorsConfigFile);
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenConfigFileWithAnimationsWithoutAnimationTexturePathField_shouldThrowInvalidAnimatorsConfigFile)
{
    ASSERT_THROW(
        animatorsSettingsReader.readAnimatorsSettings(configWithAnimatorWithoutAnimationTexturePathField),
        exceptions::InvalidAnimatorsConfigFile);
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenConfigFileWithAnimationsWithoutNumberOfAnimationsField_shouldThrowInvalidAnimatorsConfigFile)
{
    ASSERT_THROW(animatorsSettingsReader.readAnimatorsSettings(
                     configWithAnimatorWithoutNumberOfAnimationTexturesField),
                 exceptions::InvalidAnimatorsConfigFile);
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenConfigFileWithAnimationsWithoutTimeBetweenTexturesField_shouldThrowInvalidAnimatorsConfigFile)
{
    ASSERT_THROW(
        animatorsSettingsReader.readAnimatorsSettings(configWithAnimatorWithoutTimeBetweenTexturesField),
        exceptions::InvalidAnimatorsConfigFile);
}

TEST_F(AnimatorSettingsYamlReaderTest, givenValidConfigWithOneAnimator_shouldReturnAnimatorsSettings)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(validPathWithOneAnimator);

    ASSERT_EQ(actualAnimatorsSettings, animatorsSettings1);
}

TEST_F(AnimatorSettingsYamlReaderTest, givenValidConfigWithTwoAnimators_shouldReturnAnimatorsSettings)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(validPathWithTwoAnimators);

    ASSERT_EQ(actualAnimatorsSettings, animatorsSettings2);
}

TEST_F(AnimatorSettingsYamlReaderTest, givenValidConfigWithSingleFileAnimator_shouldReturnAnimatorsSettings)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(validPathWithSingleFileAnimator);

    ASSERT_EQ(actualAnimatorsSettings, bunnyAnimatorsSettings);
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenValidConfigWithSingleFileAnimatorWithMissingFields_shouldThrowInvalidAnimatorsConfigFile)
{
    ASSERT_THROW(
        animatorsSettingsReader.readAnimatorsSettings(validPathWithSingleFileAnimatorWithMissingFields),
        exceptions::InvalidAnimatorsConfigFile);
}
