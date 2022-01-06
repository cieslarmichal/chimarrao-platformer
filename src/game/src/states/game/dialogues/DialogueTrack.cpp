#include "DialogueTrack.h"

#include "exceptions/InvalidDialogueTrack.h"

namespace game
{

DialogueTrack::DialogueTrack(std::vector<DialogueEntry> dialogueEntries)
    : entries{std::move(dialogueEntries)}, currentEntryIndex{-1}
{
    if (entries.empty())
    {
        throw exceptions::InvalidDialogueTrack{"dialogue track is empty"};
    }
}

std::optional<DialogueEntry> DialogueTrack::getNextDialogue() const
{
    if (currentEntryIndex + 1 < static_cast<int>(entries.size()))
    {
        currentEntryIndex++;
        return entries[currentEntryIndex];
    }

    return std::nullopt;
}
}