#pragma once

#include <memory>

#include "DialoguesParser.h"
#include "FileAccess.h"
#include "DialoguesReader.h"

namespace game
{
class DefaultDialoguesReader : public DialoguesReader
{
public:
    explicit DefaultDialoguesReader(std::shared_ptr<utils::FileAccess>);

    DialogueTrack read(const std::string& dialogueTrackPath) const override;

private:
    std::shared_ptr<utils::FileAccess> fileAccess;
    DialoguesParser dialoguesParser;
};
}
