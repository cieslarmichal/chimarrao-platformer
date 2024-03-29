#include "DefaultTileMap.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "TileMapSerializerMock.h"

using namespace game;
using namespace ::testing;

namespace
{
const std::string mapName{"name"};
const std::string newMapName{"new_name"};
const std::string serializedMap{"serialized_map"};
const std::string mapPath{utils::ProjectPathReader::getProjectRootPath() + "maps/custom/name.map"};
const utils::Vector2i mapSize{3, 4};
const utils::Vector2i positionOnMap{2, 2};
const utils::Vector2i newMapSize{2, 2};
const utils::Vector2i positionOnNewMap{0, 1};
const TileMapInfo newTileMapInfoWithTiles{
    newMapName,
    newMapSize,
    {{std::make_shared<TileInfo>(std::optional<TileType>{std::nullopt}),
      std::make_shared<TileInfo>(std::optional<TileType>{std::nullopt})},
     {std::make_shared<TileInfo>(std::optional<TileType>{TileType::Grass}),
      std::make_shared<TileInfo>(std::optional<TileType>{std::nullopt})}}};
}

class DefaultTileMapTest : public Test
{
public:
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    std::unique_ptr<StrictMock<TileMapSerializerMock>> mapSerializerInit =
        std::make_unique<StrictMock<TileMapSerializerMock>>();
    StrictMock<TileMapSerializerMock>* mapSerializer = mapSerializerInit.get();
    DefaultTileMap tileMap{mapName, mapSize, std::move(mapSerializerInit), fileAccess};
};

TEST_F(DefaultTileMapTest, getName_shouldReturnMapName)
{
    auto name = tileMap.getName();

    EXPECT_EQ(mapName, name);
}

TEST_F(DefaultTileMapTest, setName_giveNewNam_getNameShouldReturnNewName)
{
    tileMap.setName(newMapName);
    auto name = tileMap.getName();

    EXPECT_EQ(newMapName, name);
}

TEST_F(DefaultTileMapTest, saveToFile_shouldUserSerializerAndFileAccess)
{
    EXPECT_CALL(*mapSerializer, serialize(_)).WillOnce(Return(serializedMap));
    EXPECT_CALL(*fileAccess, write(mapPath, serializedMap));

    tileMap.saveToFile();
}

TEST_F(DefaultTileMapTest, getTile_givePositionOnMap_shouldReturnEmptyTile)
{
    auto tile = tileMap.getTile(positionOnMap);

    EXPECT_EQ(std::nullopt, tile->type);
}

TEST_F(DefaultTileMapTest, setTile_givePositionOnMapAndBrickType_getterShouldReturnBrickType)
{
    tileMap.setTile(positionOnMap, TileType::Brick);
    auto tile = tileMap.getTile(positionOnMap);

    EXPECT_EQ(TileType::Brick, tile->type);
}

TEST_F(DefaultTileMapTest, setTileMapInfo_giveNewTileMapInfoWithTiles_shouldSetAllValues)
{
    tileMap.setTileMapInfo(newTileMapInfoWithTiles);
    auto& tile = tileMap.getTile(positionOnNewMap);
    auto name = tileMap.getName();

    EXPECT_EQ(TileType::Grass, tile->type);
    EXPECT_EQ(newMapName, name);
}