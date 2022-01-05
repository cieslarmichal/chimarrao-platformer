#include "DialoguesValidator.h"

#include "DialogueActor.h"
#include "StringHelper.h"

namespace game
{
namespace
{
const std::string actorWithStatementSeparator{":"};

const auto separatorNotFound = [](std::string::size_type separatorPosition)
{ return separatorPosition == std::string::npos; };
const auto statementNotFound = [](const std::string& dialogueLine, std::string::size_type separatorPosition)
{ return separatorPosition + 2 >= dialogueLine.size(); };
const auto spaceBetweenActorAndStatementNotFound =
    [](const std::string& dialogueLine, std::string::size_type separatorPosition)
{
    const auto expectedSpacePosition = separatorPosition + 1;
    return dialogueLine[expectedSpacePosition] != ' ';
};
}

bool DialoguesValidator::validate(const std::vector<std::string>& dialogueLines) const
{
    if (dialogueLines.empty())
    {
        return false;
    }

    for (const auto& dialogueLine : dialogueLines)
    {
        if (dialogueLine.empty())
        {
            return false;
        }

        const auto separatorPosition = dialogueLine.find(actorWithStatementSeparator);

        if (separatorNotFound(separatorPosition) or statementNotFound(dialogueLine, separatorPosition))
        {
            return false;
        }

        if (spaceBetweenActorAndStatementNotFound(dialogueLine, separatorPosition))
        {
            return false;
        }

        const auto actorName = utils::StringHelper::substring(dialogueLine, 0, separatorPosition);

        const auto validActorName = dialogueActors.contains(actorName);

        if (not validActorName)
        {
            return false;
        }
    }

    return true;
}
}