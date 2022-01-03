#include "Level1Controller.h"

#include "gtest/gtest.h"

#include "RendererPoolMock.h"
#include "StatesMock.h"
#include "WindowMock.h"
#include "TileMapMock.h"
#include "FileAccessMock.h"
#include "InputMock.h"

using namespace game;
using namespace components::core;
using namespace ::testing;

class Level1ControllerTest_Base : public Test
{
public:
    Level1ControllerTest_Base()
    {
        EXPECT_CALL(*tileMap, loadFromFile(_));
    }

    std::shared_ptr<StrictMock<window::WindowMock>> window =
        std::make_shared<StrictMock<window::WindowMock>>();
    std::shared_ptr<StrictMock<utils::FileAccessMock>> fileAccess =
        std::make_shared<StrictMock<utils::FileAccessMock>>();
    std::shared_ptr<StrictMock<graphics::RendererPoolMock>> rendererPool =
        std::make_shared<StrictMock<graphics::RendererPoolMock>>();
    StrictMock<StatesMock> states;
    const utils::DeltaTime deltaTime{1.0};
    StrictMock<input::InputMock> input;
    std::shared_ptr<StrictMock<TileMapMock>> tileMap = std::make_shared<StrictMock<TileMapMock>>();
    const utils::Vector2f position1{20, 20};
    const utils::Vector2f position2{22, 20};
    const utils::Vector2f position3{25, 20};
    std::shared_ptr<components::core::SharedContext> sharedContext =
        std::make_shared<components::core::SharedContext>(rendererPool);
    std::shared_ptr<ComponentOwner> player = std::make_shared<ComponentOwner>(position1, "Level1aControllerTest_Base1", sharedContext);
    std::shared_ptr<ComponentOwner> rabbit = std::make_shared<ComponentOwner>(position2, "Level1aControllerTest_Base2", sharedContext);
    std::shared_ptr<ComponentOwner> npc = std::make_shared<ComponentOwner>(position3, "Level1aControllerTest_Base3", sharedContext);
    std::shared_ptr<Level1MainCharacters> mainCharacters = std::make_shared<Level1MainCharacters>(player, rabbit, npc);
};

class Level1ControllerTest : public Level1ControllerTest_Base
{
public:
    Level1Controller levelController{tileMap, mainCharacters};
};

TEST_F(Level1ControllerTest, update_shouldReturnFalse)
{
    ASSERT_EQ(levelController.update(), false);
}
