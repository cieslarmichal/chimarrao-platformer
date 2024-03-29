#include "FileSystemMapsReader.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"

#include "ProjectPathReader.h"

using namespace game;
using namespace ::testing;

class FileSystemMapsReaderTest : public Test
{
public:
    const std::string mapsDirectory = utils::ProjectPathReader::getProjectRootPath() + "maps/custom";
    const std::vector<std::string> filePaths{"xxx", "yyyy"};
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    FileSystemMapsReader reader{fileAccess};
};

TEST_F(FileSystemMapsReaderTest, readMapFilePaths)
{
    EXPECT_CALL(*fileAccess, getAllPathsFromDirectory(mapsDirectory)).WillOnce(Return(filePaths));

    const auto actualFilePaths = reader.readMapFilePaths();

    ASSERT_EQ(actualFilePaths, filePaths);
}