#pragma once

#include <boost/optional.hpp>

#include "FilePath.h"

namespace utils
{
class IncrementalFilePathsCreator
{
public:
    static  FilePaths createFilePaths(const FilePath& fullPathToFirstFile,
                                               unsigned numberOfIncrementalFiles);

private:
    static boost::optional<std::string> extractDirectory(const FilePath&);
    static boost::optional<std::string> extractFileName(const FilePath&);
    static boost::optional<int> extractNumberFromFileName(const FilePath&);
};
}