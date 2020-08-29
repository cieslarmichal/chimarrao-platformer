#include "IncrementalFilePathsCreator.h"

#include "gtest/gtest.h"

using namespace utils;
using namespace ::testing;

namespace
{
const auto emptyFilePath{""};
const auto filePathWithoutNumberInFileName{"something/file.txt"};
const auto filePathWithoutFileNameSeparator{"xxxx"};
const auto filePathWithMultipleNumbersInFileName{"something/33file11.txt"};
const auto filePathWithFileNameSeparator{"something/file11.txt"};
const FilePaths filePaths{"something/file11.txt", "something/file12.txt", "something/file13.txt"};
}

class IncrementalFilePathsCreatorTest : public Test
{
public:
    IncrementalFilePathsCreator filePathsCreator;
};

TEST_F(IncrementalFilePathsCreatorTest, givenEmptyFilePath_shouldReturnEmptyFilePaths)
{
    const auto actualFilePaths = filePathsCreator.createFilePaths(emptyFilePath, 3);

    ASSERT_TRUE(actualFilePaths.empty());
}

TEST_F(IncrementalFilePathsCreatorTest, givenFilePathWithoutFileNameSeparator_shouldReturnEmptyFilePaths)
{
    const auto actualFilePaths = filePathsCreator.createFilePaths(filePathWithoutFileNameSeparator, 3);

    ASSERT_TRUE(actualFilePaths.empty());
}

TEST_F(IncrementalFilePathsCreatorTest, givenFilePathWithoutNumberInFileName_shouldReturnEmptyFilePaths)
{
    const auto actualFilePaths = filePathsCreator.createFilePaths(filePathWithoutNumberInFileName, 3);

    ASSERT_TRUE(actualFilePaths.empty());
}

TEST_F(IncrementalFilePathsCreatorTest, givenFilePathWithMultipleNumbersInFileName_shouldReturnEmptyFilePaths)
{
    const auto actualFilePaths = filePathsCreator.createFilePaths(filePathWithMultipleNumbersInFileName, 3);

    ASSERT_TRUE(actualFilePaths.empty());
}

TEST_F(IncrementalFilePathsCreatorTest, givenValidFilePathAndZeroNumberOfFile_shouldEmptyFilePaths)
{
    const auto actualFilePaths = filePathsCreator.createFilePaths(filePathWithFileNameSeparator, 0);

    ASSERT_TRUE(actualFilePaths.empty());
}

TEST_F(IncrementalFilePathsCreatorTest,
       givenValidFilePathAndNonZeroValue_shouldReturnFilePathsWithIncrementedNames)
{
    const auto actualFilePaths = filePathsCreator.createFilePaths(filePathWithFileNameSeparator, 3);

    ASSERT_EQ(actualFilePaths, filePaths);
}