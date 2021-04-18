#pragma once

#include "FileAccess.h"

namespace utils
{
class DefaultFileAccess : public FileAccess
{
public:
    void write(const std::string& absolutePath, const std::string& content) const override;
    void append(const std::string& absolutePath, const std::string& content) const override;
    std::string readContent(const std::string& absolutePath) const override;
    void createDirectory(const std::string& absolutePath) const override;
    void remove(const std::string& absolutePath) const override;
    void rename(const std::string& absolutePath, const std::string& newAbsolutePath) const override;
    bool exists(const std::string& absolutePath) const override;
    bool isRegularFile(const std::string& absolutePath) const override;
    bool isDirectory(const std::string& absolutePath) const override;
};
}