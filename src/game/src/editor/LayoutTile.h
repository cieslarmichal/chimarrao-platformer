#pragma once

#include <memory>

#include "Tile.h"
#include "TileMap.h"
#include "Timer.h"
#include "core/ClickableComponent.h"
#include "core/ComponentOwner.h"
#include "core/GraphicsComponent.h"
#include "core/MouseOverComponent.h"

namespace game
{
class LayoutTile
{
public:
    LayoutTile(const std::shared_ptr<graphics::RendererPool>&, const utils::Vector2i& position,
               const utils::Vector2f& size, TileType& currentTileType, TileMap& tileMap);

    void update(const utils::DeltaTime&, const input::Input&);
    void activate();
    void deactivate();
    void pause();
    void moveTile(utils::Vector2f);
    const utils::Vector2f& getPosition() const;
    bool isActive() const;

private:
    std::shared_ptr<components::core::ComponentOwner> componentOwner;
    utils::Timer freezeClickableTileTimer;
    const float timeAfterTileCanBeClicked;
};
}