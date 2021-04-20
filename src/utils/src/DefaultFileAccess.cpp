#include "DefaultFileAccess.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "ProjectPathReader.h"
#include "exceptions/DirectoryNotFound.h"
#include "exceptions/FileNotFound.h"
#include "exceptions/PathIsNotAFile.h"

namespace fs = std::filesystem;

namespace utils
{
namespace
{
enum class Result
{
    Success,
    Failure
};

Result tryToWrite(std::ofstream& fileStream, const std::string& data);

const std::string directoryNotFoundMessage{"Directory not found "};
const std::string fileNotFoundMessage{"File not found "};
const std::string fileNotFoundReadingMessage{fileNotFoundMessage + "while reading: "};
const std::string fileNotFoundWritingMessage{fileNotFoundMessage + "while writing: "};
const std::string fileNotFoundAppendingMessage{fileNotFoundMessage + "while appending: "};
}

void DefaultFileAccess::write(const std::string& absolutePath, const std::string& content) const
{
    std::ofstream fileStream{absolutePath};

    if (tryToWrite(fileStream, content) == Result::Failure)
    {
        throw exceptions::FileNotFound(fileNotFoundWritingMessage + absolutePath);
    }
}

void DefaultFileAccess::append(const std::string& absolutePath, const std::string& content) const
{
    std::ofstream fileStream{absolutePath, std::ofstream::app};

    if (tryToWrite(fileStream, content) == Result::Failure)
    {
        throw exceptions::FileNotFound(fileNotFoundAppendingMessage + absolutePath);
    }
}

std::string DefaultFileAccess::readContent(const std::string& absolutePath) const
{
    std::ifstream fileStream{absolutePath};
    std::stringstream buffer;

    if (fileStream.is_open())
    {
        buffer << fileStream.rdbuf();
    }
    else
    {
        throw exceptions::FileNotFound(fileNotFoundReadingMessage + absolutePath);
    }

    return buffer.str();
}

void DefaultFileAccess::createDirectory(const std::string& absolutePath) const
{
    fs::create_directory(absolutePath);
}

void DefaultFileAccess::remove(const std::string& absolutePath) const
{
    fs::remove_all(absolutePath);
}

void DefaultFileAccess::rename(const std::string& absolutePath, const std::string& newAbsolutePath) const
{
    if (!exists(absolutePath))
    {
        return;
    }
    fs::rename(absolutePath, newAbsolutePath);
}

bool DefaultFileAccess::exists(const std::string& absolutePath) const
{
    return fs::exists(absolutePath);
}

bool DefaultFileAccess::isRegularFile(const std::string& absolutePath) const
{
    return fs::is_regular_file(absolutePath);
}

bool DefaultFileAccess::isDirectory(const std::string& absolutePath) const
{
    return fs::is_directory(absolutePath);
}

std::string DefaultFileAccess::getFileName(const std::string& pathInit) const
{
    if (not isRegularFile(pathInit))
    {
        throw exceptions::PathIsNotAFile{pathInit + " is not a file"};
    }

    fs::path path(pathInit);
    return path.filename().string();
}

std::string DefaultFileAccess::getFileNameWithoutExtension(const std::string& pathInit) const
{
    if (not isRegularFile(pathInit))
    {
        throw exceptions::PathIsNotAFile{pathInit + " is not a file"};
    }

    fs::path path(pathInit);
    return path.stem().string();
}

std::vector<std::string>
DefaultFileAccess::getAllPathsFromDirectory(const std::string& directoryPathInit) const
{
    fs::path directoryPath(directoryPathInit);

    if (not fs::exists(directoryPath) || not isDirectory(directoryPath.string()))
    {
        throw exceptions::DirectoryNotFound{directoryNotFoundMessage + directoryPathInit};
    }

    std::vector<std::string> filesInsideDirectory;

    for (auto& file : fs::recursive_directory_iterator(directoryPath))
    {
        filesInsideDirectory.push_back(file.path().string());
    }

    return filesInsideDirectory;
}

namespace
{

Result tryToWrite(std::ofstream& fileStream, const std::string& data)
{
    if (fileStream.is_open())
    {
        fileStream << data;
        return Result::Success;
    }
    return Result::Failure;
}

}
}