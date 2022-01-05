#pragma once

#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "StringHelper.h"

namespace components::core
{
enum class DialogueActor
{
    Player,
    Rabbit,
    Druid,
};

const std::unordered_set<std::string> dialogueActors{"player", "rabbit", "druid"};

inline std::string toString(DialogueActor dialogueActor)
{
    std::unordered_map<DialogueActor, std::string> dialogueActorToString{{DialogueActor::Player, "Player"},
                                                                         {DialogueActor::Rabbit, "Rabbit"},
                                                                         {DialogueActor::Druid, "Druid"}};

    return dialogueActorToString.at(dialogueActor);
}

inline DialogueActor toDialogueActor(const std::string& dialogueActorAsString)
{
    const auto dialogueActorAsLowerString = utils::StringHelper::getLowerCases(dialogueActorAsString);

    std::unordered_map<std::string, DialogueActor> stringToDialogueActor{{"player", DialogueActor::Player},
                                                                         {"rabbit", DialogueActor::Rabbit},
                                                                         {"druid", DialogueActor::Druid}};

    return stringToDialogueActor.at(dialogueActorAsLowerString);
}

inline std::ostream& operator<<(std::ostream& os, DialogueActor dialogueActor)
{
    return os << "DialogueActor::" << toString(dialogueActor);
}
}