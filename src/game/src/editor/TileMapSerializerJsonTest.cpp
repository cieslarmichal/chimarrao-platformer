#include "TileMapSerializerJson.h"

#include "gtest/gtest.h"

#include "Tile.h"

using namespace game;
using namespace ::testing;

namespace
{

}

class TileMapSerializerJsonTest : public Test
{
public:
    TileMapSerializerJson serializer;
    const TileMapInfo validTileMapInfo{"name", {1, 1}, {{std::make_shared<Tile>(Tile{std::nullopt})}}};
    const std::string validTileMapInfoJson = "";
};

TEST_F(TileMapSerializerJsonTest, giveTileMapInfo_shouldReturnSerializedTileMapInfo)
{
    auto serializedTileMapInfo = serializer.serialize(validTileMapInfo);

    EXPECT_EQ(validTileMapInfoJson, serializedTileMapInfo);
}