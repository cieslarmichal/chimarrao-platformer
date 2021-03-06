#include "EditorState.h"

#include <core/MouseOverComponent.h>

#include "EditorMenuState.h"
#include "GetProjectPath.h"
#include "TileMap.h"
#include "core/ComponentOwner.h"
#include "core/GraphicsComponent.h"

namespace game
{
namespace
{
const float rendererPoolSizeX = 80;
const float rendererPoolSizeY = 60;
const float tileSizeX = 4;
const float tileSizeY = 4;
const auto pathToBackground =
    utils::getProjectPath("chimarrao-platformer") + "resources/BG/background_glacial_mountains.png";
const auto pathToBrickTileTexture =
    utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png";
const auto tilesTextureVector =
    std::vector<std::string>{utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png",
                             utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/2.png"};
}

EditorState::EditorState(const std::shared_ptr<window::Window>& windowInit,
                         const std::shared_ptr<input::InputManager>& inputManagerInit,
                         const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                         std::stack<std::unique_ptr<State>>& states)
    : State{windowInit, inputManagerInit, rendererPoolInit, states},
      inputStatus{nullptr},
      paused{false},
      timeAfterStateCouldBePaused{0.5f},
      timeAfterButtonsCanBeClicked{0.3f}
{
    inputManager->registerObserver(this);

    currentTileId = 0;
    currentTilePath = tilesTextureVector[currentTileId];
    background = std::make_unique<components::core::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::core::GraphicsComponent>(
        rendererPool, utils::Vector2f{rendererPoolSizeX, rendererPoolSizeY}, utils::Vector2f{0, 0},
        pathToBackground, graphics::VisibilityLayer::Background);
    background->addComponent<components::core::HitBoxComponent>(
        utils::Vector2f{rendererPoolSizeX, rendererPoolSizeY});
    const auto changeBlockAction = [&]() {
        std::cout << currentTilePath << std::endl;
        currentTileId = currentTileId + 1 < tilesTextureVector.size() ? currentTileId + 1 : 0;
        currentTilePath = tilesTextureVector[currentTileId];
    };
    background->addComponent<components::core::ClickableComponent>(
        inputManager,
        std::vector<components::core::KeyAction>{{input::InputKey::MouseRight, changeBlockAction}});

    tileMap = std::make_unique<TileMap>(
        utils::Vector2i(rendererPoolSizeX / tileSizeX, rendererPoolSizeY / tileSizeY),
        utils::Vector2f(tileSizeX, tileSizeY));
    for (int y = 0; y < rendererPoolSizeY / tileSizeY; ++y)
    {
        for (int x = 0; x < rendererPoolSizeX / tileSizeX; ++x)
        {
            auto clickableTile = std::make_shared<components::core::ComponentOwner>(
                utils::Vector2f{static_cast<float>(x * tileSizeX), static_cast<float>(y * tileSizeY)});
            auto graphicsComponent = clickableTile->addComponent<components::core::GraphicsComponent>(
                rendererPool, utils::Vector2f{tileSizeX, tileSizeY},
                utils::Vector2f{static_cast<float>(x * tileSizeX), static_cast<float>(y * tileSizeY)},
                pathToBrickTileTexture, graphics::VisibilityLayer::Invisible);
            clickableTile->addComponent<components::core::HitBoxComponent>(
                utils::Vector2f{tileSizeX, tileSizeY});
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
            clickableTile->addComponent<components::core::ClickableComponent>(inputManager,
                                                                              actionOnClickBlock);
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
            clickableTile->addComponent<components::core::MouseOverComponent>(
                inputManager, actionOnMouseOverBlock, actionOnMouseOut);
            clickableTileMap.push_back(clickableTile);
        }
    }

    initialize();
}

EditorState::~EditorState()
{
    inputManager->removeObserver(this);
}

void EditorState::initialize()
{
    background->loadDependentComponents();

    for (auto& tile : clickableTileMap)
    {
        tile->loadDependentComponents();
        tile->getComponent<components::core::ClickableComponent>()->disable();
    }
}

void EditorState::update(const utils::DeltaTime& dt)
{
    if (buttonsActionsFrozen &&
        freezeClickableButtonsTimer.getElapsedSeconds() > timeAfterButtonsCanBeClicked)
    {
        unfreezeButtons();
    }

    if (pauseTimer.getElapsedSeconds() > timeAfterStateCouldBePaused &&
        inputStatus->isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (not paused)
    {
        background->update(dt);
        for (auto& tile : clickableTileMap)
        {
            tile->update(dt);
        }
    }
}

void EditorState::lateUpdate(const utils::DeltaTime& dt)
{
    if (not paused)
    {
        background->lateUpdate(dt);
        for (auto& tile : clickableTileMap)
        {
            tile->lateUpdate(dt);
        }
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
    paused = false;
    freezeClickableButtonsTimer.restart();
    pauseTimer.restart();

    for (auto& tile : clickableTileMap)
    {
        tile->enable();
        tile->getComponent<components::core::ClickableComponent>()->disable();
    }
}

void EditorState::deactivate()
{
    active = false;
    pauseTimer.restart();
}

void EditorState::handleInputStatus(const input::InputStatus& inputStatusInit)
{
    inputStatus = &inputStatusInit;
}

void EditorState::pause()
{
    paused = true;
    buttonsActionsFrozen = true;

    for (auto& tile : clickableTileMap)
    {
        tile->disable();
        tile->getComponent<components::core::GraphicsComponent>()->enable();
    }

    states.push(std::make_unique<EditorMenuState>(window, inputManager, rendererPool, states));
}

void EditorState::unfreezeButtons()
{
    buttonsActionsFrozen = false;
    for (auto& tile : clickableTileMap)
    {
        tile->getComponent<components::core::ClickableComponent>()->enable();
    }
}

}