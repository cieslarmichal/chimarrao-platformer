#include "DefaultDialoguesReader.h"

namespace game
{

DefaultDialoguesReader::DefaultDialoguesReader(std::shared_ptr<utils::FileAccess> fileAccessInit)
    : fileAccess{std::move(fileAccessInit)}
{
}

std::vector<DialogueEntry> DefaultDialoguesReader::read(const std::string& dialogueTrackPath) const
{
    const auto fileContent = fileAccess->readContent(dialogueTrackPath);

    return dialoguesParser.parse(fileContent);
}
}