#include "DialoguesParser.h"

#include "gtest/gtest.h"

#include "exceptions/FileNotFound.h"
#include "exceptions/InvalidDialogueTrack.h"

using namespace game;
using namespace ::testing;

namespace
{
const std::string nonExistingFilePath{"nonExisting.txt"};
const std::string emptyDialogueFilePath{"emptyFile.txt"};
const std::string invalidDialogueFilePath1{"invalid1.txt"};
const std::string invalidDialogueFilePath2{"invalid2.txt"};
const std::string validDialogueFilePath{"valid.txt"};
const std::string emptyContent;
const std::string invalidContent1{"druid: Oh, you found me\n"
                                  "player: Hi\n"
                                  "Do you know where can I find some food?\n"
                                  "druid: Have you seen those bushes?\n"
                                  "player: Yes, I guess so"};
const std::string invalidContent2{"druid: Oh, you found me\n"
                                  "player: Hi\n"
                                  "player Do you know where can I find some food?\n"
                                  "druid: Have you seen those bushes?\n"
                                  "player: Yes, I guess so"};
const std::string validContent{"druid: Oh, you found me\n"
                               "player: Hi\n"
                               "player: Do you know where can I find some food?\n"
                               "druid: Have you seen those bushes?\n"
                               "player: Yes, I guess so"};
const DialogueTrack expectedDialogueTrack{{DialogueActor::Druid, "Oh, you found me"},
                                          {DialogueActor::Player, "Hi"},
                                          {DialogueActor::Player, "Do you know where can I find some food?"},
                                          {DialogueActor::Druid, "Have you seen those bushes?"},
                                          {DialogueActor::Player, "Yes, I guess so"}};
}

class DialoguesParserTest : public Test
{
public:
    DialoguesParser parser{};
};

TEST_F(DialoguesParserTest, parseEmptyFile_shouldThrow)
{
    ASSERT_THROW(parser.parse(emptyContent), exceptions::InvalidDialogueTrack);
}

TEST_F(DialoguesParserTest, parseFileContentWithoutActorsSpecified_shouldThrow)
{
    ASSERT_THROW(parser.parse(invalidContent1), exceptions::InvalidDialogueTrack);
}

TEST_F(DialoguesParserTest, parseFileContentWithoutSeparatorSpecified_shouldThrow)
{
    ASSERT_THROW(parser.parse(invalidContent2), exceptions::InvalidDialogueTrack);
}

TEST_F(DialoguesParserTest, parseValidFileContent_shouldReturnDialogueTrack)
{
    const auto dialogueTrack = parser.parse(validContent);

    ASSERT_EQ(dialogueTrack, expectedDialogueTrack);
}
