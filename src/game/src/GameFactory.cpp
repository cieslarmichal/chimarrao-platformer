#include "GameFactory.h"

#include "AudioFactory.h"
#include "DefaultStates.h"
#include "FileAccessFactory.h"
#include "GraphicsFactory.h"
#include "InputManagerFactory.h"
#include "WindowFactory.h"
#include "DefaultTileMap.h"
#include "TileMapSerializerJson.h"

namespace game
{
namespace
{
const int tileSizeX = 4;
const int tileSizeY = 4;
const auto windowSize = utils::Vector2u{800, 600};
const auto mapSize = utils::Vector2u{80u, 60u};
const auto gameTitle = "chimarrao-platformer";
}

std::unique_ptr<Game> GameFactory::createGame()
{
    const auto graphicsFactory = graphics::GraphicsFactory::createGraphicsFactory();
    const auto windowFactory = window::WindowFactory::createWindowFactory();
    const auto inputManagerFactory = input::InputManagerFactory::createInputManagerFactory();
    const auto fileAccessFactory = utils::FileAccessFactory::createFileAccessFactory();
    const auto audioFactory = audio::AudioFactory::createAudioFactory();

    std::shared_ptr<window::Window> window = windowFactory->createWindow(windowSize, gameTitle);

    std::shared_ptr<graphics::RendererPool> rendererPool =
        graphicsFactory->createRendererPool(window, windowSize, mapSize);

    std::shared_ptr<input::InputManager> inputManager = inputManagerFactory->createInputManager(window);

    std::shared_ptr<utils::FileAccess> fileAccess = fileAccessFactory->createDefaultFileAccess();

    std::shared_ptr<audio::MusicManager> musicManager = audioFactory->createMusicManager();

    auto tileMap = std::make_shared<DefaultTileMap>(
        "",
        utils::Vector2i(static_cast<int>(mapSize.x) / tileSizeX * 2, static_cast<int>(mapSize.y) / tileSizeY),
        std::make_unique<TileMapSerializerJson>(), fileAccess);

    auto states = std::make_unique<DefaultStates>(window, rendererPool, fileAccess, tileMap, musicManager);

    return std::make_unique<Game>(window, inputManager, std::move(states));
}
}