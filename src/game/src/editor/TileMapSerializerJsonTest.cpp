#include "TileMapSerializerJson.h"

#include "gtest/gtest.h"

#include "exceptions/InvalidField.h"
#include "exceptions/InvalidMapName.h"
#include "exceptions/InvalidMapRepresentation.h"
#include "exceptions/RequiredFieldNotFound.h"
#include "exceptions/WrongMapSize.h"

using namespace game;
using namespace ::testing;

class TileMapSerializerJsonTest : public Test
{
public:
    TileMapSerializerJson serializer;
    const TileMapInfo validTileMapInfo{
        "name",
        {3, 3},
        {{std::make_shared<Tile>(Tile{std::nullopt}), std::make_shared<Tile>(Tile{std::nullopt}),
          std::make_shared<Tile>(Tile{std::nullopt})},
         {std::make_shared<Tile>(Tile{TileType::Grass}), std::make_shared<Tile>(Tile{TileType::Grass}),
          std::make_shared<Tile>(Tile{TileType::Grass})},
         {std::make_shared<Tile>(Tile{TileType::Brick}), std::make_shared<Tile>(Tile{TileType::Brick}),
          std::make_shared<Tile>(Tile{TileType::Brick})}}};
    const std::string validTileMapInfoJson{
        R"({"data":{"tiles":[[0,0,0],[1,1,1],[2,2,2]]},"info":{"mapSize":{"x":3,"y":3},"name":"name"}})"};
    const TileMapInfo invalidTileMapInfo_noName{"", {1, 1}, {{std::make_shared<Tile>(Tile{std::nullopt})}}};
    const TileMapInfo invalidTileMapInfo_wrongSize{
        "name", {21, 37}, {{std::make_shared<Tile>(Tile{std::nullopt})}}};
    const std::string invalidTileMapInfoJson_invalidJson{"there is no json"};
    const std::string invalidTileMapInfoJson_noInfo{R"({"data":{"tiles":[[0]]}})"};
    const std::string invalidTileMapInfoJson_noName{
        R"({"data":{"tiles":[[0]]},"info":{"mapSize":{"x":1,"y":1}}})"};
    const std::string invalidTileMapInfoJson_invalidName{
        R"({"data":{"tiles":[[0]]},"info":{"mapSize":{"x":1,"y":1},"name":{"x":1,"y":1}}})"};
    const std::string invalidTileMapInfoJson_noMapSize{R"({"data":{"tiles":[[0]]},"info":{"name":"name"}})"};
    const std::string invalidTileMapInfoJson_invalidMapSize{
        R"({"data":{"tiles":[[0]]},"info":{"mapSize":"mapSize","name":"name"}})"};
    const std::string invalidTileMapInfoJson_noData{R"({"info":{"mapSize":{"x":1,"y":1},"name":"name"}})"};
    const std::string invalidTileMapInfoJson_differentMapSizes{
        R"({"data":{"tiles":[[0]]},info":{"mapSize":{"x":21,"y":37},"name":"name"}})"};

    const std::string validTileMapInfo_realMap{
        "{\"data\":{\"tiles\":[[2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
        "0,0],[0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,2,1,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0],[0,0,0,0,0,0,2,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,"
        "0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
        "0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0,0,0,0,0,0,"
        "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]]},\"info\":{\"mapSize\":{\"x\":40,\"y\":15},"
        "\"name\":\"schody\"}}"};
};

TEST_F(TileMapSerializerJsonTest, giveValidTileMapInfo_shouldReturnSerializedTileMapInfo)
{
    auto serializedTileMapInfo = serializer.serialize(validTileMapInfo);

    EXPECT_EQ(validTileMapInfoJson, serializedTileMapInfo);
}

TEST_F(TileMapSerializerJsonTest, giveInvalidTileMapInfoWithNoName_shouldThrowException)
{
    EXPECT_THROW(serializer.serialize(invalidTileMapInfo_noName), exceptions::InvalidMapName);
}

TEST_F(TileMapSerializerJsonTest, giveInvalidTileMapInfoWithWrongMapSize_shouldThrowException)
{
    EXPECT_THROW(serializer.serialize(invalidTileMapInfo_wrongSize), exceptions::WrongMapSize);
}

TEST_F(TileMapSerializerJsonTest, giveValidTileMapJson_shouldReturnDeserializedTileMapInfo)
{
    auto deserializedTileMapInfo = serializer.deserialize(validTileMapInfoJson);

    EXPECT_EQ(validTileMapInfo.name, deserializedTileMapInfo.name);
    EXPECT_EQ(validTileMapInfo.mapSize, deserializedTileMapInfo.mapSize);
    for (int y = 0; y < validTileMapInfo.tiles.size(); y++)
    {
        for (int x = 0; x < validTileMapInfo.tiles[y].size(); x++)
        {
            EXPECT_EQ(validTileMapInfo.tiles[x][y]->type, deserializedTileMapInfo.tiles[x][y]->type);
        }
    }
}

TEST_F(TileMapSerializerJsonTest,
       giveInvalidTileMapInfoJson_invalidJson_shouldThrowExceptionInvalidMapRepresentation)
{
    EXPECT_THROW(serializer.deserialize(invalidTileMapInfoJson_invalidJson),
                 exceptions::InvalidMapRepresentation);
}

TEST_F(TileMapSerializerJsonTest, giveInvalidTileMapInfoJson_noInfo_shouldThrowExceptionRequiredFieldNotFound)
{
    ASSERT_THROW(serializer.deserialize(invalidTileMapInfoJson_noInfo), exceptions::RequiredFieldNotFound);
}

TEST_F(TileMapSerializerJsonTest, giveValidTileMapInfoJson_deserializeAndSerialize_shouldReturnTheSameJson)
{
    auto deserializedTileMapInfo = serializer.deserialize(validTileMapInfoJson);
    auto serializedTileMapInfo = serializer.serialize(deserializedTileMapInfo);

    EXPECT_EQ(validTileMapInfoJson, serializedTileMapInfo);
}

TEST_F(TileMapSerializerJsonTest, giveValidRealTileMapInfoJson_deserialize_shouldReturnTileMapInfo)
{
    auto deserializedTileMapInfo = serializer.deserialize(validTileMapInfo_realMap);
    ASSERT_EQ(deserializedTileMapInfo.mapSize.y,deserializedTileMapInfo.tiles.size());
    ASSERT_EQ(deserializedTileMapInfo.mapSize.x,deserializedTileMapInfo.tiles[0].size());
    ASSERT_TRUE(deserializedTileMapInfo.tiles[1][1]->type == TileType::Brick);
}