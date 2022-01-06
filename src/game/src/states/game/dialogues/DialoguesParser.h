#pragma once

#include "DialogueTrack.h"
#include "DialoguesValidator.h"

namespace game
{
class DialoguesParser
{
public:
    std::vector<DialogueEntry> parse(const std::string& dialoguesFileContent) const;

private:
    std::vector<DialogueEntry> parse(const std::vector<std::string>& dialogueLines) const;

    DialoguesValidator dialoguesValidator;
};
}
