#include "DialoguesReader.h"

namespace game
{

DialoguesReader::DialoguesReader(std::shared_ptr<utils::FileAccess> fileAccessInit)
    : fileAccess{std::move(fileAccessInit)}
{
}

DialogueTrack DialoguesReader::read(const std::string& dialogueTrackPath) const
{
    const auto fileContent = fileAccess->readContent(dialogueTrackPath);

    return dialoguesParser.parse(fileContent);
}
}