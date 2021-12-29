#pragma once

#include <vector>

#include "State.h"
#include "Timer.h"
#include "UIConfig.h"
#include "UIManager.h"
#include "editor/LayoutTile.h"
#include "editor/TileMap.h"

namespace game
{
class EditorStateUIConfigBuilder;

class EditorState : public State
{
public:
    friend class EditorStateUIConfigBuilder;

    explicit EditorState(const std::shared_ptr<window::Window>&,
                         const std::shared_ptr<graphics::RendererPool>&, std::shared_ptr<utils::FileAccess>,
                         States&, std::shared_ptr<components::ui::UIManager>, std::shared_ptr<TileMap>,
                         std::unique_ptr<utils::Timer>,
                         const std::shared_ptr<components::core::SharedContext>&);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;
    void pause();

private:
    void setTileMap();

    bool paused;
    std::unique_ptr<utils::Timer> pauseTimer;
    std::unique_ptr<utils::Timer> moveTimer;
    const float timeAfterStateCouldBePaused;
    const float timeBetweenTileMoves;
    std::vector<std::shared_ptr<components::core::ComponentOwner>> clickableTileMap;
    std::shared_ptr<TileType> currentTileType;
    std::vector<LayoutTile> layoutTileMap;
    std::shared_ptr<TileMap> tileMap;
    std::shared_ptr<components::ui::UIManager> uiManager;
    const std::shared_ptr<components::core::SharedContext>& sharedContext;
};
}