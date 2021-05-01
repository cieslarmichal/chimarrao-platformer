#include "FileSystemMapsReader.h"

#include "ProjectPathReader.h"

namespace game
{

FileSystemMapsReader::FileSystemMapsReader(std::shared_ptr<utils::FileAccess> fileAccessInit)
    : fileAccess{std::move(fileAccessInit)}
{
}

std::vector<std::string> FileSystemMapsReader::readMapFilePaths()
{
    static const std::string mapsDirectory = utils::ProjectPathReader::getProjectRootPath() + "maps";
    return fileAccess->getAllPathsFromDirectory(mapsDirectory);
}
}