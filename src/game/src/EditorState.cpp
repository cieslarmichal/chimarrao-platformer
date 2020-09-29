#include "EditorState.h"

#include "ComponentOwner.h"
#include "GetProjectPath.h"
#include "GraphicsComponent.h"
#include "TileMap.h"

namespace game
{

EditorState::EditorState(const std::shared_ptr<window::Window>& windowInit,
                         const std::shared_ptr<input::InputManager>& inputManagerInit,
                         const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                         std::stack<std::unique_ptr<State>>& states)
    : State{windowInit, inputManagerInit, rendererPoolInit, states}
{
    background = std::make_unique<components::ComponentOwner>(utils::Vector2f{0, 0});
    background->addComponent<components::GraphicsComponent>(
        rendererPool, utils::Vector2f{80, 60}, utils::Vector2f{0, 0},
        utils::getProjectPath("chimarrao-platformer") + "resources/BG/background_glacial_mountains.png",
        graphics::VisibilityLayer::Background);

    tileMap = std::make_unique<TileMap>(utils::Vector2i(16, 12), utils::Vector2f(5, 5));

    for (int y = 0; y < 12; ++y)
    {
        for (int x = 0; x < 16; ++x)
        {
            auto clickableTile = std::make_unique<components::ComponentOwner>(
                utils::Vector2f{static_cast<float>(x * 5), static_cast<float>(y * 5)});
            auto graphicsComponent = clickableTile->addComponent<components::GraphicsComponent>(
                rendererPool, utils::Vector2f{5, 5},
                utils::Vector2f{static_cast<float>(x * 5), static_cast<float>(y * 5)},
                utils::getProjectPath("chimarrao-platformer") + "resources/Tiles/brick.png",
                graphics::VisibilityLayer::Invisible);
            clickableTile->addComponent<components::HitboxComponent>(utils::Vector2f{5, 5});
            clickableTile->addComponent<components::ClickableComponent>(inputManager, [&]() {
                tileMap->setTile({x, y}, 1);
                graphicsComponent->setVisibility(graphics::VisibilityLayer::First);
            });
            clickableTileMap.push_back(std::move(clickableTile));
        }
    }

    initialize();
}
void EditorState::initialize()
{
    for (auto& tile : clickableTileMap)
    {
        tile->loadDependentComponents();
        tile->start();
    }
}
void EditorState::update(const utils::DeltaTime& dt)
{
    for (auto& tile : clickableTileMap)
    {
        tile->update(dt);
    }
}
void EditorState::lateUpdate(const utils::DeltaTime& dt)
{
    for (auto& tile : clickableTileMap)
    {
        tile->lateUpdate(dt);
    }
}
void EditorState::render()
{
    rendererPool->renderAll();
}
}