#include "DialogueTrack.h"

#include "gtest/gtest.h"

#include "exceptions/InvalidDialogueTrack.h"

using namespace game;
using namespace ::testing;

namespace
{
const std::vector<DialogueEntry> emptyDialogueEntries;
const DialogueEntry entry1{components::core::DialogueActor::Druid, "Oh, you found me"};
const DialogueEntry entry2{components::core::DialogueActor::Player, "Hi"};
const DialogueEntry entry3{components::core::DialogueActor::Player,
                           "Do you know where can I find some food?"};
const std::vector<DialogueEntry> dialogueEntries{entry1, entry2, entry3};
}

class DialogueTrackTest : public Test
{
public:
    DialogueTrack track{dialogueEntries};
};

TEST_F(DialogueTrackTest, givenEmptyDialogueEntries_shouldThrowInvalidDialogueTrack)
{
    ASSERT_THROW(DialogueTrack{emptyDialogueEntries}, exceptions::InvalidDialogueTrack);
}

TEST_F(DialogueTrackTest, getNextDialogue_shouldReturnConsecutiveDialogues)
{
    const auto dialogueEntry1 = track.getNextDialogue();
    const auto dialogueEntry2 = track.getNextDialogue();
    const auto dialogueEntry3 = track.getNextDialogue();

    ASSERT_EQ(dialogueEntry1, entry1);
    ASSERT_EQ(dialogueEntry2, entry2);
    ASSERT_EQ(dialogueEntry3, entry3);
}

TEST_F(DialogueTrackTest, getNextDialogueIfThereIsNoMoreDialogueEntries_shouldReturnNone)
{
    track.getNextDialogue();
    track.getNextDialogue();
    track.getNextDialogue();

    const auto entry = track.getNextDialogue();

    ASSERT_EQ(entry, std::nullopt);
}
