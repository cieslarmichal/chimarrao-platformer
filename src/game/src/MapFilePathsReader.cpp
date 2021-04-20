#include "MapFilePathsReader.h"

#include "ProjectPathReader.h"

namespace game
{

MapFilePathsReader::MapFilePathsReader(std::shared_ptr<utils::FileAccess> fileAccessInit)
    : fileAccess{std::move(fileAccessInit)}
{
}

std::vector<std::string> MapFilePathsReader::readMapFilePaths()
{
    static const std::string mapsDirectory = utils::ProjectPathReader::getProjectRootPath() + "maps";
    return fileAccess->getAllPathsFromDirectory(mapsDirectory);
}
}