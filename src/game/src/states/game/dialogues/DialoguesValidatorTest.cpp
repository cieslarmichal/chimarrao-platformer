#include "DialoguesValidator.h"

#include "gtest/gtest.h"

using namespace game;
using namespace ::testing;

namespace
{
const std::vector<std::string> emptyDialogueLines{};
const std::vector<std::string> dialogueLinesWithoutActor{{": Hello"}, {"player: Hi"}};
const std::vector<std::string> dialogueLinesWithoutActorAndSeparator{{"Hello"}, {"player: Hi"}};
const std::vector<std::string> dialogueLinesWithInvalidActor{{"bear: Hello"}, {"player: Hi"}};
const std::vector<std::string> dialogueLinesWithEmptyLine{{"druid: Hello"}, {""}, {"player: Hi"}};
const std::vector<std::string> dialogueLinesWithNoSpaceBetweenSeparatorAndStatement{{"druid:"},
                                                                                    {"player: Hi"}};
const std::vector<std::string> dialogueLinesWithEmptyStatement{{"druid: "}, {"player: Hi"}};
const std::vector<std::string> validDialogueLines{{"druid: Oh, you found me"},
                                                  {"player: Hi"},
                                                  {"player: Do you know where can I find some food?"},
                                                  {"druid: Have you seen those bushes?"},
                                                  {"player: Yes, I guess so"}};
}

class DialoguesValidatorTest : public Test
{
public:
    DialoguesValidator validator;
};

TEST_F(DialoguesValidatorTest, givenEmptyDialogueLines_shouldReturnFalse)
{
    ASSERT_FALSE(validator.validate(emptyDialogueLines));
}

TEST_F(DialoguesValidatorTest, givenDialogueLinesWithNoActor_shouldReturnFalse)
{
    ASSERT_FALSE(validator.validate(dialogueLinesWithoutActor));
}

TEST_F(DialoguesValidatorTest, givenDialogueLinesWithNoActorAndNoSeparator_shouldReturnFalse)
{
    ASSERT_FALSE(validator.validate(dialogueLinesWithoutActorAndSeparator));
}

TEST_F(DialoguesValidatorTest, givenDialogueLinesWithInvalidActor_shouldReturnFalse)
{
    ASSERT_FALSE(validator.validate(dialogueLinesWithInvalidActor));
}

TEST_F(DialoguesValidatorTest, givenDialogueLinesWithEmptyLine_shouldReturnFalse)
{
    ASSERT_FALSE(validator.validate(dialogueLinesWithEmptyLine));
}

TEST_F(DialoguesValidatorTest, givenDialogueLinesWithNoSpaceAfterSeparator_shouldReturnFalse)
{
    ASSERT_FALSE(validator.validate(dialogueLinesWithNoSpaceBetweenSeparatorAndStatement));
}

TEST_F(DialoguesValidatorTest, givenDialogueLinesWithEmptyStatement_shouldReturnFalse)
{
    ASSERT_FALSE(validator.validate(dialogueLinesWithEmptyStatement));
}

TEST_F(DialoguesValidatorTest, givenValidDialogueLines_shouldReturnTrue)
{
    ASSERT_TRUE(validator.validate(validDialogueLines));
}
