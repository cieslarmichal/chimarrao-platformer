#pragma once

#include <memory>
#include <string>
#include <vector>

#include "FileAccess.h"

namespace game
{
class MapFilePathsReader
{
public:
    explicit MapFilePathsReader(std::shared_ptr<utils::FileAccess>);

    std::vector<std::string> readMapFilePaths();

private:
    const std::shared_ptr<utils::FileAccess> fileAccess;
};
}