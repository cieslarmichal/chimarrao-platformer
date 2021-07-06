#include "FileSystemMapsReader.h"

#include <algorithm>
#include <iterator>

#include "ProjectPathReader.h"

namespace game
{

const std::string FileSystemMapsReader::mapsDirectory{utils::ProjectPathReader::getProjectRootPath() +
                                                      "maps"};

FileSystemMapsReader::FileSystemMapsReader(std::shared_ptr<utils::FileAccess> fileAccessInit)
    : fileAccess{std::move(fileAccessInit)}
{
}

std::vector<std::string> FileSystemMapsReader::readMapFilePaths()
{
    auto allMapFilePaths = fileAccess->getAllPathsFromDirectory(mapsDirectory);
    std::sort(allMapFilePaths.begin(), allMapFilePaths.end());
    return allMapFilePaths;
}

std::vector<std::string> FileSystemMapsReader::readMapNames()
{
    const auto mapFilePaths = readMapFilePaths();
    std::vector<std::string> mapNames;
    std::transform(mapFilePaths.begin(), mapFilePaths.end(), std::back_inserter(mapNames),
                   [&](const std::string& mapFile)
                   { return fileAccess->getFileNameWithoutExtension(mapFile); });
    return mapNames;
}
}