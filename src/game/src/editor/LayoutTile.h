#pragma once

#include <memory>

#include "TileInfo.h"
#include "TileMap.h"
#include "Timer.h"
#include "ClickableComponent.h"
#include "ComponentOwner.h"
#include "GraphicsComponent.h"
#include "MouseOverComponent.h"

namespace game
{
class LayoutTile
{
public:
    LayoutTile(const std::shared_ptr<components::core::SharedContext>&, const utils::Vector2i& position,
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
    std::unique_ptr<utils::Timer> freezeClickableTileTimer;
    const float timeAfterTileCanBeClicked;
};
}