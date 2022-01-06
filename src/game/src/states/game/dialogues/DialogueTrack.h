#pragma once

#include <vector>

#include "DialogueEntry.h"

namespace game
{
class DialogueTrack
{
public:
    explicit DialogueTrack(std::vector<DialogueEntry>);

    std::optional<DialogueEntry> getNextDialogue() const;

private:
    std::vector<DialogueEntry> entries;
    mutable int currentEntryIndex;
};
}