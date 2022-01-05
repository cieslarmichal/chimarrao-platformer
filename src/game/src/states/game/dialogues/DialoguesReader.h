#pragma once

#include "DialogueTrack.h"
#include "FileAccess.h"
#include <memory>
#include "DialoguesParser.h"

namespace game
{
class DialoguesReader
{
public:
    explicit DialoguesReader(std::shared_ptr<utils::FileAccess>);

    DialogueTrack read(const std::string& dialogueTrackPath) const;

private:
    std::shared_ptr<utils::FileAccess> fileAccess;
    DialoguesParser dialoguesParser;
};
}
