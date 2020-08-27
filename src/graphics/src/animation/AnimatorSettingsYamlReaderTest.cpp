#include "AnimatorSettingsYamlReader.h"

#include "animation/exceptions/AnimatorsConfigFileNotFound.h"

#include "gtest/gtest.h"

#include "GetProjectPath.h"

using namespace ::testing;
using namespace graphics;
using namespace animation;

namespace
{
const std::string projectPath{utils::getProjectPath("chimarrao")};
const std::string testDirectory = projectPath + "src/graphics/src/animation/testFiles/";
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

const AnimationSettings playerAnimationSettings1{"idle", "resources/Player/Idle/idle-with-weapon-1.png", 6, 0.3};
const AnimationSettings playerAnimationSettings2{"walk", "resources/Player/Walk/walk-with-weapon-1.png", 11, 0.7};
const AnimationsSettings playerAnimationsSettings{playerAnimationSettings1, playerAnimationSettings2};
const AnimationSettings enemyAnimationSettings1{"idle", "resources/Enemy/Idle/idle-with-weapon-1.png", 3, 0.4};
const AnimationSettings enemyAnimationSettings2{"walk", "resources/Enemy/Walk/walk-with-weapon-1.png", 2, 0.8};
const AnimationsSettings enemyAnimationsSettings{enemyAnimationSettings1, enemyAnimationSettings2};
const AnimatorsSettings animatorsSettings1{{"player", playerAnimationsSettings}};
const AnimatorsSettings animatorsSettings2{{"player", playerAnimationsSettings},
                                           {"enemy", enemyAnimationsSettings}};
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

TEST_F(AnimatorSettingsYamlReaderTest, givenInvalidYamlFile_shouldReturnNone)
{
    const auto actualAnimatorsSettings = animatorsSettingsReader.readAnimatorsSettings(invalidYaml);

    ASSERT_EQ(actualAnimatorsSettings, boost::none);
}

TEST_F(AnimatorSettingsYamlReaderTest, givenConfigFileWithoutAnimatorsField_shouldReturnNone)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(configWithoutAnimatorsField);

    ASSERT_EQ(actualAnimatorsSettings, boost::none);
}

TEST_F(AnimatorSettingsYamlReaderTest, givenConfigFileWithEmptyAnimators_shouldReturnEmptySettings)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(configWithEmptyAnimators);

    ASSERT_TRUE(actualAnimatorsSettings->empty());
}

TEST_F(AnimatorSettingsYamlReaderTest, givenConfigFileWithAnimatorWithoutAnimatorNameField_shouldReturnNone)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(configWithAnimatorWithoutAnimatorNameField);

    ASSERT_EQ(actualAnimatorsSettings, boost::none);
}

TEST_F(AnimatorSettingsYamlReaderTest, givenConfigFileWithAnimatorWithoutAnimationsField_shouldReturnNone)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(configWithAnimatorWithoutAnimationsField);

    ASSERT_EQ(actualAnimatorsSettings, boost::none);
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenConfigFileWithAnimationsWithoutAnimationTypeField_shouldReturnNone)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(configWithAnimatorWithoutAnimationTypeField);

    ASSERT_EQ(actualAnimatorsSettings, boost::none);
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenConfigFileWithAnimationsWithoutAnimationTexturePathField_shouldReturnNone)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(configWithAnimatorWithoutAnimationTexturePathField);

    ASSERT_EQ(actualAnimatorsSettings, boost::none);
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenConfigFileWithAnimationsWithoutNumberOfAnimationsField_shouldReturnNone)
{
    const auto actualAnimatorsSettings = animatorsSettingsReader.readAnimatorsSettings(
        configWithAnimatorWithoutNumberOfAnimationTexturesField);

    ASSERT_EQ(actualAnimatorsSettings, boost::none);
}

TEST_F(AnimatorSettingsYamlReaderTest,
       givenConfigFileWithAnimationsWithoutTimeBetweenTexturesField_shouldReturnNone)
{
    const auto actualAnimatorsSettings = animatorsSettingsReader.readAnimatorsSettings(
        configWithAnimatorWithoutTimeBetweenTexturesField);

    ASSERT_EQ(actualAnimatorsSettings, boost::none);
}

TEST_F(AnimatorSettingsYamlReaderTest, givenValidConfigWithOneAnimator_shouldReturnAnimatorsSettings)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(validPathWithOneAnimator);

    ASSERT_EQ(*actualAnimatorsSettings, animatorsSettings1);
}

TEST_F(AnimatorSettingsYamlReaderTest, givenValidConfigWithTwoAnimators_shouldReturnAnimatorsSettings)
{
    const auto actualAnimatorsSettings =
        animatorsSettingsReader.readAnimatorsSettings(validPathWithTwoAnimators);

    ASSERT_EQ(*actualAnimatorsSettings, animatorsSettings2);
}
