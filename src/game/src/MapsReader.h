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
};
}