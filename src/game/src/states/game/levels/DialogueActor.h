#pragma once

#include <ostream>
#include <string>
#include <unordered_map>

#include "StringHelper.h"

namespace game
{
enum class DialogueActor
{
    Player,
    Rabbit,
    Druid,
};

inline std::string toString(DialogueActor dialogueActorType)
{
    std::unordered_map<DialogueActor, std::string> dialogueActorTypeToString{
        {DialogueActor::Player, "Player"},
        {DialogueActor::Rabbit, "Rabbit"},
        {DialogueActor::Druid, "Druid"}};

    return dialogueActorTypeToString.at(dialogueActorType);
}

inline DialogueActor toDialogueActorType(const std::string& dialogueActorTypeAsString)
{
    const auto dialogueActorTypeAsLowerString = utils::StringHelper::getLowerCases(dialogueActorTypeAsString);

    std::unordered_map<std::string, DialogueActor> stringToDialogueActorType{
        {"player", DialogueActor::Player},
        {"rabbit", DialogueActor::Rabbit},
        {"druid", DialogueActor::Druid}};

    return stringToDialogueActorType.at(dialogueActorTypeAsLowerString);
}

inline std::ostream& operator<<(std::ostream& os, DialogueActor dialogueActorType)
{
    return os << "DialogueActorType::" << toString(dialogueActorType);
}
}