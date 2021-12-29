#include "EditorState.h"

#include <utility>

#include "DefaultUIManager.h"
#include "EditorMenuState.h"
#include "EditorStateUIConfigBuilder.h"
#include "TimerFactory.h"

namespace game
{
namespace
{
const int rendererPoolSizeX = 80;
const int rendererPoolSizeY = 60;
const int tileSizeX = 4;
const int tileSizeY = 4;
}

EditorState::EditorState(const std::shared_ptr<window::Window>& windowInit,
                         const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                         std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                         std::shared_ptr<components::ui::UIManager> uiManagerInit,
                         std::shared_ptr<TileMap> tileMapInit, std::unique_ptr<utils::Timer> moveTimer,
                         const std::shared_ptr<components::core::SharedContext>& sharedContext)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      paused{false},
      moveTimer{std::move(moveTimer)},
      timeAfterStateCouldBePaused{0.5f},
      timeBetweenTileMoves{0.005f},
      currentTileType{std::make_shared<TileType>(defaultTileType)},
      tileMap{std::move(tileMapInit)},
      uiManager{std::move(uiManagerInit)},
      sharedContext{sharedContext}
{
    uiManager->createUI(EditorStateUIConfigBuilder::createEditorUIConfig(this));
    tileMap->setTileMapInfo(TileMapInfo{"", tileMap->getSize(), {}});
    setTileMap();

    pauseTimer = utils::TimerFactory::createTimer();
}

NextState EditorState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (pauseTimer->getElapsedSeconds() > timeAfterStateCouldBePaused &&
        input.isKeyPressed(input::InputKey::Escape))
    {
        pause();
    }

    if (moveTimer->getElapsedSeconds() > timeBetweenTileMoves && input.isKeyPressed(input::InputKey::Left) &&
        layoutTileMap.front().getPosition().x < 0)
    {
        moveTimer->restart();
        for (auto& layoutTile : layoutTileMap)
        {
            layoutTile.moveTile({0.3f, 0.f});
        }
    }
    else if (moveTimer->getElapsedSeconds() > timeBetweenTileMoves &&
             input.isKeyPressed(input::InputKey::Right))
    {
        if (layoutTileMap.back().getPosition().x + tileSizeX > rendererPoolSizeX)
        {
            moveTimer->restart();
            for (auto& layoutTile : layoutTileMap)
            {
                layoutTile.moveTile({-0.3f, 0.f});
            }
        }
        else
        {
            const auto tileMapSizeBeforeExtend = tileMap->getSize().x;

            tileMap->extend();

            for (int y = 0; y < tileMap->getSize().y; ++y)
            {
                for (int x = tileMapSizeBeforeExtend; x < tileMap->getSize().x; ++x)
                {
                    auto tileTypeOpt = tileMap->getTile(utils::Vector2i{x, y})->type;
                    auto& tileType = tileTypeOpt ? *tileTypeOpt : *currentTileType;
                    layoutTileMap.emplace_back(LayoutTile{sharedContext, utils::Vector2i{x, y},
                                                          utils::Vector2f{tileSizeX, tileSizeY}, tileType,
                                                          *tileMap});
                    layoutTileMap.back().moveTile({-80.f, 0.f});
                }
            }
        }
    }

    if (not paused)
    {
        for (auto& layoutTile : layoutTileMap)
        {
            layoutTile.update(deltaTime, input);
        }
        uiManager->update(deltaTime, input);
    }

    return NextState::Same;
}

void EditorState::lateUpdate(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (not paused)
    {
        for (auto& tile : clickableTileMap)
        {
            tile->lateUpdate(deltaTime, input);
        }
    }
}

void EditorState::render()
{
    rendererPool->renderAll();
}

StateType EditorState::getType() const
{
    return StateType::Editor;
}

void EditorState::activate()
{
    active = true;
    paused = false;
    pauseTimer->restart();

    for (auto& layoutTile : layoutTileMap)
    {
        layoutTile.activate();
    }
    uiManager->activate();

    setTileMap();
}

void EditorState::deactivate()
{
    active = false;
    pauseTimer->restart();
    for (auto& layoutTile : layoutTileMap)
    {
        layoutTile.deactivate();
    }
    uiManager->deactivate();
}

void EditorState::pause()
{
    paused = true;

    for (auto& layoutTile : layoutTileMap)
    {
        layoutTile.pause();
    }

    states.addNextState(StateType::EditorMenu);
}

void EditorState::setTileMap()
{
    layoutTileMap.clear();

    for (int y = 0; y < tileMap->getSize().y; ++y)
    {
        for (int x = 0; x < tileMap->getSize().x; ++x)
        {
            layoutTileMap.emplace_back(LayoutTile{sharedContext, utils::Vector2i{x, y},
                                                  utils::Vector2f{tileSizeX, tileSizeY}, *currentTileType,
                                                  *tileMap});
        }
    }
}

}