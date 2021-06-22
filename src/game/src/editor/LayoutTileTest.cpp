#include "LayoutTile.h"

#include "gtest/gtest.h"

#include "FileAccessMock.h"
#include "InputMock.h"
#include "RendererPoolMock.h"
#include "TileMapMock.h"
#include "TileMapSerializerMock.h"

using namespace game;
using namespace ::testing;

namespace
{
const utils::Vector2i positionInit{0, 0};
const utils::Vector2f position{0, 0};
const utils::Vector2f size{0, 0};
const utils::Vector2f offset{1, 1};
const utils::Vector2f newPosition{1, 1};
std::shared_ptr<Tile> tile = std::make_shared<Tile>();

const std::string mapName{"name"};
const utils::Vector2i mapSize{1, 1};
const auto grassTexturePath = utils::ProjectPathReader::getProjectRootPath() + "resources/Tiles/2.png";
}

class LayoutTileTest_Base : public Test
{
public:
    LayoutTileTest_Base()
    {
        EXPECT_CALL(*tileMap, getTile(positionInit)).WillRepeatedly(ReturnRef(tile));
        EXPECT_CALL(*rendererPool, acquire(size, utils::Vector2f(position), grassTexturePath,
                                           graphics::VisibilityLayer::Invisible, _));
        EXPECT_CALL(*rendererPool, setTexture(_, _, _));
        EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::Invisible)).Times(2);
        EXPECT_CALL(*rendererPool, release(_));
    }

    std::shared_ptr<graphics::RendererPoolMock> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();

    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    std::unique_ptr<StrictMock<TileMapSerializerMock>> mapSerializerInit =
        std::make_unique<StrictMock<TileMapSerializerMock>>();
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
    TileType currentTileType = TileType::Brick;
    const utils::DeltaTime deltaTime{1.0};
    NiceMock<input::InputMock> input;
};

class LayoutTileTest : public LayoutTileTest_Base
{
public:
    LayoutTile layoutTile{rendererPool, positionInit, size, currentTileType, *tileMap};
};

TEST_F(LayoutTileTest, isActive_shouldReturnTrue)
{
    EXPECT_EQ(true, layoutTile.isActive());
}

TEST_F(LayoutTileTest, deactivate_isActiveShouldReturnFalse)
{
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::Invisible));
    layoutTile.deactivate();

    EXPECT_EQ(false, layoutTile.isActive());
}

TEST_F(LayoutTileTest, activate_isActiveShouldReturnTrue)
{
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::Invisible)).Times(2);

    layoutTile.deactivate();
    layoutTile.activate();

    EXPECT_EQ(true, layoutTile.isActive());
}

TEST_F(LayoutTileTest, getPosition)
{
    const auto& actualPosition = layoutTile.getPosition();

    EXPECT_EQ(position, actualPosition);
}

TEST_F(LayoutTileTest, moveTile_shouldUpdatePosition)
{
    layoutTile.moveTile(offset);

    auto actualPosition = layoutTile.getPosition();

    EXPECT_EQ(newPosition, actualPosition);
}

TEST_F(LayoutTileTest, update_shouldNotThrow)
{
    EXPECT_CALL(*rendererPool, setPosition(_, position));

    ASSERT_NO_THROW(layoutTile.update(deltaTime, input));
}

TEST_F(LayoutTileTest, pause_shouldDeactivateTile)
{
    EXPECT_CALL(*rendererPool, setVisibility(_, graphics::VisibilityLayer::Invisible)).Times(2);

    layoutTile.pause();

    EXPECT_EQ(false, layoutTile.isActive());
}
