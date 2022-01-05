#include "DialoguesParser.h"

#include "StringHelper.h"
#include "exceptions/InvalidDialogueTrack.h"

namespace game
{
namespace
{
const std::string actorWithStatementSeparator{":"};
}

DialogueTrack DialoguesParser::parse(const std::string& dialoguesFileContent) const
{
    const auto dialogueLines = utils::StringHelper::split(dialoguesFileContent);

    const auto dialogueLinesAreValid = dialoguesValidator.validate(dialogueLines);

    if (not dialogueLinesAreValid)
    {
        throw exceptions::InvalidDialogueTrack{"Dialogue lines are not valid"};
    }

    return parse(dialogueLines);
}

DialogueTrack DialoguesParser::parse(const std::vector<std::string>& dialogueLines) const
{
    DialogueTrack dialogueTrack;

    for (const auto& dialogueLine : dialogueLines)
    {
        const auto separatorPosition = dialogueLine.find(actorWithStatementSeparator);
        const auto actorName = utils::StringHelper::substring(dialogueLine, 0, separatorPosition);
        const auto statement =
            utils::StringHelper::substring(dialogueLine, separatorPosition + 2, dialogueLine.size());
        const auto dialogueActor = toDialogueActor(actorName);
        dialogueTrack.push_back({dialogueActor, statement});
    }

    return dialogueTrack;
}

}
