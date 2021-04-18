#include "DefaultFileAccess.h"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "GetProjectPath.h"
#include "exceptions/DirectoryNotFound.h"
#include "exceptions/FileNotFound.h"

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