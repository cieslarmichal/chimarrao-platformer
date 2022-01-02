#pragma once

#include <string>
#include <vector>

namespace game
{
class MapsReader
{
public:
    virtual ~MapsReader() = default;

    virtual std::vector<std::string> readMapFilePaths() = 0;
    virtual std::vector<std::string> readMapNames() = 0;
};
}