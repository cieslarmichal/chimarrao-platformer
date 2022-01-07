#pragma once

#include <memory>

#include "DialoguesParser.h"
#include "DialoguesReader.h"
#include "FileAccess.h"

namespace game
{
class DefaultDialoguesReader : public DialoguesReader
{
public:
    explicit DefaultDialoguesReader(std::shared_ptr<utils::FileAccess>);

    std::vector<DialogueEntry> read(const std::string& dialogueTrackPath) const override;

private:
    std::shared_ptr<utils::FileAccess> fileAccess;
    DialoguesParser dialoguesParser;
};
}
