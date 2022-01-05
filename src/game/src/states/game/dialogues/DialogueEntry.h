#pragma once

#include <ostream>
#include <string>
#include <tuple>

#include "DialogueActor.h"

namespace game
{
struct DialogueEntry
{
    components::core::DialogueActor actor;
    std::string statement;
};

inline bool operator==(const DialogueEntry& lhs, const DialogueEntry& rhs)
{
    auto tieStruct = [](const DialogueEntry& dialogueEntry)
    { return std::tie(dialogueEntry.actor, dialogueEntry.statement); };
    return tieStruct(lhs) == tieStruct(rhs);
}

inline std::string toString(const DialogueEntry& dialogueEntry)
{
    return "{" + toString(dialogueEntry.actor) + ", " + dialogueEntry.statement + "}";
}

inline std::ostream& operator<<(std::ostream& os, const DialogueEntry& dialogueEntry)
{
    return os << toString(dialogueEntry);
}
}