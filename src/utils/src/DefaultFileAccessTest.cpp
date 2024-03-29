#include "DefaultFileAccess.h"

#include "gtest/gtest.h"

#include "ProjectPathReader.h"
#include "StlOperators.h"
#include "exceptions/DirectoryNotFound.h"
#include "exceptions/FileNotFound.h"
#include "exceptions/PathIsNotAFile.h"

using namespace ::testing;
using namespace utils;

namespace
{
const std::string projectPath{ProjectPathReader::getProjectRootPath()};

#ifdef _WIN32
const std::string testDirectory = projectPath + R"(src/utils/src/testDirectory/testFiles/)";
const std::string testExperimentalDirectory =
    projectPath + R"(src\utils\src\testDirectory\testExperimental\)";
const std::string slashDelimiter = "\\";
#else
const std::string testDirectory = projectPath + "src/utils/src/testDirectory/testFiles/";
const std::string testExperimentalDirectory = projectPath + "src/utils/src/testDirectory/testExperimental/";
const std::string slashDelimiter = "/";
#endif

const std::string textToWrite{"Hello this text should be written\nby write method"};
const std::string textToAppend{"\nand this text should be written\nby append method"};
const std::string textAfterWriteAndAppend{textToWrite + textToAppend};
const std::string exampleContent{"this is example file created\nin order to check readContent\nmethod"};
const std::string filenameForReading = "testFileForReading.txt";
const std::string filenameForReadingWithoutExtension = "testFileForReading";
const std::string filenameForWriting = "testFileForWriting.txt";
const std::string dummyDirectoryName{"dummyDir"};
const std::string fileInsideDummyDir{"fileInsideDir.txt"};
const std::string jpgFile{"jpgFile.jpg"};
const std::string pdfFile{"pdfFile.pdf"};
const std::string pathForReading{testDirectory + filenameForReading};
const std::string pathForWriting{testDirectory + filenameForWriting};
const std::string dummyDirectoryPath{testDirectory + dummyDirectoryName};
const std::string fileInsideDummyDirPath{dummyDirectoryPath + slashDelimiter + fileInsideDummyDir};
const std::string jpgPath{testDirectory + jpgFile};
const std::string pdfPath{testDirectory + pdfFile};
const std::vector<std::string> expectedFilePaths{pathForReading,         pathForWriting, dummyDirectoryPath,
                                                 fileInsideDummyDirPath, jpgPath,        pdfPath};
const std::string newDirectoryPath{testExperimentalDirectory + "xxx/"};
const std::string newFileInDirectoryPath{newDirectoryPath + "aaaa.txt"};
const std::string newDirectoryChangedPath{testExperimentalDirectory + "yyy/"};
const std::string newFilePath{testExperimentalDirectory + "xxx.txt"};
const std::string newFileChangedPath{testExperimentalDirectory + "yyy.txt"};
const std::string incorrectPath = "433\\UTzxxxx/fi123xtF";
}

class DefaultFileAccessTest : public Test
{
public:
    void prepareCreatedFile(const std::string& path) const
    {
        fileAccess.write(path, "");
    }

    void prepareCreatedDirectory(const std::string& path) const
    {
        fileAccess.createDirectory(path);
    }

    void prepareDeletedPath(const std::string& path) const
    {
        fileAccess.remove(path);
    }

    DefaultFileAccess fileAccess;
};

TEST_F(DefaultFileAccessTest, givenCorrectPath_shouldWriteToFile)
{
    fileAccess.write(pathForWriting, textToWrite);

    const auto actualFileContent = fileAccess.readContent(pathForWriting);

    ASSERT_EQ(actualFileContent, textToWrite);
}

TEST_F(DefaultFileAccessTest, givenIncorrectPath_shouldThrowFileNotFoundForWritingAppendingAndReading)
{
    ASSERT_THROW(fileAccess.write(incorrectPath, textToWrite), utils::exceptions::FileNotFound);
    ASSERT_THROW(fileAccess.append(incorrectPath, textToWrite), utils::exceptions::FileNotFound);
    ASSERT_THROW(fileAccess.readContent(incorrectPath), utils::exceptions::FileNotFound);
}

TEST_F(DefaultFileAccessTest, givenCorrectPath_shouldAppendToFile)
{
    fileAccess.write(pathForWriting, textToWrite);
    fileAccess.append(pathForWriting, textToAppend);

    const auto actualFileContent = fileAccess.readContent(pathForWriting);

    ASSERT_EQ(actualFileContent, textAfterWriteAndAppend);
}

TEST_F(DefaultFileAccessTest, givenCorrectPath_shouldReturnContentOfFile)
{
    const auto actualFileContent = fileAccess.readContent(pathForReading);

    ASSERT_EQ(actualFileContent, exampleContent);
}

TEST_F(DefaultFileAccessTest, givenCorrectPath_shouldCreateNewDirectory)
{
    prepareDeletedPath(newDirectoryPath);

    fileAccess.createDirectory(newDirectoryPath);

    ASSERT_TRUE(fileAccess.exists(newDirectoryPath));
    prepareDeletedPath(newDirectoryPath);
}

TEST_F(DefaultFileAccessTest, givenExistingDirectory_shouldRemoveDirectory)
{
    prepareCreatedDirectory(newDirectoryPath);

    fileAccess.remove(newDirectoryPath);

    ASSERT_FALSE(fileAccess.exists(newDirectoryPath));
}

TEST_F(DefaultFileAccessTest, givenExistingDirectoryWithFiles_shouldRemoveDirectoryWithFiles)
{
    prepareCreatedDirectory(newDirectoryPath);
    prepareCreatedFile(newFileInDirectoryPath);

    fileAccess.remove(newDirectoryPath);

    ASSERT_FALSE(fileAccess.exists(newDirectoryPath));
    ASSERT_FALSE(fileAccess.exists(newFileInDirectoryPath));
}

TEST_F(DefaultFileAccessTest, givenExistingFile_shouldRemoveFile)
{
    prepareCreatedFile(newFilePath);

    fileAccess.remove(newFilePath);

    ASSERT_FALSE(fileAccess.exists(newFilePath));
}

TEST_F(DefaultFileAccessTest, givenNonexistingDirectory_shouldNotRenameDirectory)
{
    fileAccess.rename(newDirectoryPath, newDirectoryChangedPath);

    ASSERT_FALSE(fileAccess.exists(newDirectoryPath));
    ASSERT_FALSE(fileAccess.exists(newDirectoryChangedPath));
}

TEST_F(DefaultFileAccessTest, givenExistingDirectory_shouldRenameDirectory)
{
    prepareCreatedDirectory(newDirectoryPath);

    fileAccess.rename(newDirectoryPath, newDirectoryChangedPath);

    ASSERT_TRUE(fileAccess.exists(newDirectoryChangedPath));
    prepareDeletedPath(newDirectoryChangedPath);
}

TEST_F(DefaultFileAccessTest, givenExistingFile_shouldRenameFile)
{
    prepareCreatedDirectory(newFilePath);

    fileAccess.rename(newFilePath, newFileChangedPath);

    ASSERT_TRUE(fileAccess.exists(newFileChangedPath));
    prepareDeletedPath(newFileChangedPath);
}

TEST_F(DefaultFileAccessTest, givenExistingPath_shouldReturnTrue)
{
    ASSERT_TRUE(fileAccess.exists(testDirectory));
}

TEST_F(DefaultFileAccessTest, givenRegularFilePath_whenCheckingForRegularFile_shouldReturnTrue)
{
    ASSERT_TRUE(fileAccess.isRegularFile(pathForReading));
}

TEST_F(DefaultFileAccessTest, givenDirectoryPath_whenCheckingForRegularFile_shouldReturnFalse)
{
    ASSERT_FALSE(fileAccess.isRegularFile(dummyDirectoryPath));
}

TEST_F(DefaultFileAccessTest, givenDirectoryPath_whenCheckingForDirectory_shouldReturnTrue)
{
    ASSERT_TRUE(fileAccess.isDirectory(dummyDirectoryPath));
}

TEST_F(DefaultFileAccessTest, givenRegularFilePath_whenCheckingForDirectory_shouldReturnFalse)
{
    ASSERT_FALSE(fileAccess.isDirectory(pathForReading));
}

TEST_F(DefaultFileAccessTest, givenIncorrectPath_whenCheckingForFileName_shouldThrowPathIsNotAFile)
{
    ASSERT_THROW(fileAccess.getFileName(incorrectPath), exceptions::PathIsNotAFile);
}

TEST_F(DefaultFileAccessTest, givenDirectoryPath_whenCheckingForFileName_shouldThrowPathIsNotAFile)
{
    ASSERT_THROW(fileAccess.getFileName(testDirectory), exceptions::PathIsNotAFile);
}

TEST_F(DefaultFileAccessTest, givenRegularFilePath_whenCheckingForFileName_shouldReturnFileName)
{
    const auto actualFileName = fileAccess.getFileName(pathForReading);

    ASSERT_EQ(actualFileName, filenameForReading);
}

TEST_F(DefaultFileAccessTest,
       givenIncorrectPath_whenCheckingForFileNameWithoutExtension_shouldThrowPathIsNotAFile)
{
    ASSERT_THROW(fileAccess.getFileNameWithoutExtension(incorrectPath), exceptions::PathIsNotAFile);
}

TEST_F(DefaultFileAccessTest,
       givenDirectoryPath_whenCheckingForFileNameWithoutExtension_shouldThrowPathIsNotAFile)
{
    ASSERT_THROW(fileAccess.getFileNameWithoutExtension(testDirectory), exceptions::PathIsNotAFile);
}

TEST_F(DefaultFileAccessTest,
       givenRegularFilePath_whenCheckingForFileNameWithoutExtension_shouldReturnFileNameWithoutExtension)
{
    const auto actualFileName = fileAccess.getFileNameWithoutExtension(pathForReading);

    ASSERT_EQ(actualFileName, filenameForReadingWithoutExtension);
}

TEST_F(DefaultFileAccessTest, givenIncorrectPath_shouldThrowDirectoryNotFound)
{
    ASSERT_THROW(fileAccess.getAllPathsFromDirectory(incorrectPath), exceptions::DirectoryNotFound);
}

TEST_F(DefaultFileAccessTest, givenCorrectDirectoryPath_shouldReturnDirectoryFilePaths)
{
    const auto actualFilePaths = fileAccess.getAllPathsFromDirectory(testDirectory);

    ASSERT_TRUE(compareVectors(actualFilePaths, expectedFilePaths));
}
