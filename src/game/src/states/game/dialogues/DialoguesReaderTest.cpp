#include "DialoguesReader.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"

#include "exceptions/FileNotFound.h"

using namespace game;
using namespace ::testing;

namespace
{
const std::string nonExistingFilePath{"nonExisting.txt"};
const std::string validDialogueFilePath{"valid.txt"};
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

class DialoguesReaderTest : public Test
{
public:
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();

    DialoguesReader reader{fileAccess};
};

TEST_F(DialoguesReaderTest, fileAccessThrow_shouldThrow)
{
    EXPECT_CALL(*fileAccess, readContent(nonExistingFilePath))
        .WillOnce(Throw(utils::exceptions::FileNotFound{""}));

    ASSERT_THROW(reader.read(nonExistingFilePath), utils::exceptions::FileNotFound);
}

TEST_F(DialoguesReaderTest, readValidFile_shouldReturnDialogueTrack)
{
    EXPECT_CALL(*fileAccess, readContent(validDialogueFilePath)).WillOnce(Return(validContent));

    const auto dialogueTrack = reader.read(validDialogueFilePath);

    ASSERT_EQ(dialogueTrack, expectedDialogueTrack);
}
