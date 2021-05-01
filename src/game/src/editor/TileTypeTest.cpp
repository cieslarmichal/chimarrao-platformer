#include "TileType.h"

#include "gtest/gtest.h"

using namespace game;
using namespace ::testing;

namespace
{

}

class TileTypeTest : public Test
{
public:
    const std::string pathToBrickTexture =
        utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/brick.png";
};

TEST_F(TileTypeTest, getNextTileType_giveBrickType_shouldReturnGrass)
{
    auto tileType = getNextTileType(TileType::Brick);

    EXPECT_EQ(TileType::Grass, tileType);
}

TEST_F(TileTypeTest, tileTypeToPathTexture_giveBrickType_shouldReturnPathToBrickTexture)
{
    auto pathToTexture = tileTypeToPathTexture(TileType::Brick);

    EXPECT_EQ(pathToBrickTexture, pathToTexture);
}
