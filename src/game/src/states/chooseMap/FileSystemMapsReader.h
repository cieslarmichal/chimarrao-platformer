#pragma once

#include <memory>

#include "FileAccess.h"
#include "MapsReader.h"

namespace game
{
class FileSystemMapsReader : public MapsReader
{
public:
    explicit FileSystemMapsReader(std::shared_ptr<utils::FileAccess>);

    std::vector<std::string> readMapFilePaths() override;
    std::vector<std::string> readMapNames() override;

private:
    const std::shared_ptr<utils::FileAccess> fileAccess;

    static const std::string mapsDirectory;
};
}