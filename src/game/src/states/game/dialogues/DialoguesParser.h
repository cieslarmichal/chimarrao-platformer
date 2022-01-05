#pragma once

#include "DialogueTrack.h"
#include "DialoguesValidator.h"

namespace game
{
class DialoguesParser
{
public:
    DialogueTrack parse(const std::string& dialoguesFileContent) const;

private:
    DialogueTrack parse(const std::vector<std::string>& dialogueLines) const;

    DialoguesValidator dialoguesValidator;
};
}
