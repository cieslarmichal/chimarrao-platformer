#include "EditorState.h"

#include <MouseOverComponent.h>

#include "ComponentOwner.h"
#include "GetProjectPath.h"
#include "GraphicsComponent.h"
#include "TileMap.h"

namespace game
{
namespace
{
float rendererPoolSizeX = 80, rendererPoolSizeY = 60;
float tileSizeX = 4, tileSizeY = 4;
auto pathToBackground =
    utils::getProjectPath("chimarrao-platformer") + "resources/BG/background_glacial_mountains.png";
auto pathToBrickTileTexture = utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png";
auto tilesTextureVector = std::vector<std::string>{
    utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png",
    utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/2.png"
};
}

EditorState::EditorState(const std::shared_ptr<window::Window>& windowInit,
                         const std::shared_ptr<input::InputManager>& inputManagerInit,
                         const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                         std::stack<std::unique_ptr<State>>& states)
    : State{windowInit, inputManagerInit, rendererPoolInit, states}
{
    currentTileId = 0;
    currentTilePath = tilesTextureVector[currentTileId];
    background = std::make_unique<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(
        rendererPool, utils::Vector2f{rendererPoolSizeX, rendererPoolSizeY}, utils::Vector2f{0, 0},
        pathToBackground, graphics::VisibilityLayer::Background);
    background->addComponent<components::HitboxComponent>(
        utils::Vector2f{rendererPoolSizeX, rendererPoolSizeY});
    const auto changeBlockAction = [&]() {
        std::cout << currentTilePath << std::endl;
        currentTileId = currentTileId + 1 < tilesTextureVector.size() ? currentTileId + 1 : 0;
        currentTilePath = tilesTextureVector[currentTileId];
    };
    background->addComponent<components::ClickableComponent>(
        inputManager, std::vector<KeyAction>{{input::InputKey::MouseRight, changeBlockAction}});

    tileMap = std::make_unique<TileMap>(
        utils::Vector2i(rendererPoolSizeX / tileSizeX, rendererPoolSizeY / tileSizeY),
        utils::Vector2f(tileSizeX, tileSizeY));
    for (int y = 0; y < rendererPoolSizeY / tileSizeY; ++y)
    {
        for (int x = 0; x < rendererPoolSizeX / tileSizeX; ++x)
        {
            auto clickableTile = std::make_shared<components::ComponentOwner>(
                utils::Vector2f{static_cast<float>(x * tileSizeX), static_cast<float>(y * tileSizeY)});
            auto graphicsComponent = clickableTile->addComponent<components::GraphicsComponent>(
                rendererPool, utils::Vector2f{tileSizeX, tileSizeY},
                utils::Vector2f{static_cast<float>(x * tileSizeX), static_cast<float>(y * tileSizeY)},
                pathToBrickTileTexture, graphics::VisibilityLayer::Invisible);
            clickableTile->addComponent<components::HitboxComponent>(utils::Vector2f{tileSizeX, tileSizeY});
            const auto actionOnClickBlock = [=]() {
                if (tileMap->getTile({x, y}) == 0)
                {
                    tileMap->setTile({x, y}, 1);
                    graphicsComponent->setColor(graphics::Color(255, 255, 255, 255));
                    graphicsComponent->setOutline(0.0f, graphics::Color::Transparent);
                    graphicsComponent->setVisibility(graphics::VisibilityLayer::First);
                    graphicsComponent->setOutline(0.2f, graphics::Color::Red);
                }
                else
                {
                    tileMap->setTile({x, y}, 0);
                    graphicsComponent->setVisibility(graphics::VisibilityLayer::First);
                    graphicsComponent->setColor(graphics::Color(255, 255, 255, 64));
                    graphicsComponent->setOutline(0.2f, graphics::Color::Green);
                }
            };
            clickableTile->addComponent<components::ClickableComponent>(inputManager, actionOnClickBlock);
            const auto actionOnMouseOverBlock = [=]() {
                graphicsComponent->setVisibility(graphics::VisibilityLayer::First);
                if (tileMap->getTile({x, y}) == 0)
                {
                    graphicsComponent->setTexture(currentTilePath);
                    graphicsComponent->setColor(graphics::Color(255, 255, 255, 64));
                    graphicsComponent->setOutline(0.2f, graphics::Color::Green);
                }
                else
                {
                    graphicsComponent->setOutline(0.2f, graphics::Color::Red);
                }
            };
            const auto actionOnMouseOut = [=]() {
                if (tileMap->getTile({x, y}) == 0)
                {
                    graphicsComponent->setVisibility(graphics::VisibilityLayer::Invisible);
                }
                else
                {
                    graphicsComponent->setVisibility(graphics::VisibilityLayer::Second);
                }
                graphicsComponent->setOutline(0.0f, graphics::Color::Transparent);
            };
            clickableTile->addComponent<components::MouseOverComponent>(inputManager, actionOnMouseOverBlock,
                                                                        actionOnMouseOut);
            clickableTileMap.push_back(clickableTile);
        }
    }

    initialize();
}

void EditorState::initialize()
{
    background->loadDependentComponents();
    background->start();
    for (auto& tile : clickableTileMap)
    {
        tile->loadDependentComponents();
        tile->start();
    }
}

void EditorState::update(const utils::DeltaTime& dt)
{
    background->update(dt);
    for (auto& tile : clickableTileMap)
    {
        tile->update(dt);
    }
}

void EditorState::lateUpdate(const utils::DeltaTime& dt)
{
    background->lateUpdate(dt);
    for (auto& tile : clickableTileMap)
    {
        tile->lateUpdate(dt);
    }
}

void EditorState::render()
{
    rendererPool->renderAll();
}

std::string EditorState::getName() const
{
    return "Editor state";
}

void EditorState::activate()
{
    active = true;
}

void EditorState::deactivate()
{
    active = false;
}

}