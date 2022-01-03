#pragma once

#include <vector>

#include "ComponentOwnersManager.h"
#include "LayoutTile.h"
#include "State.h"
#include "TileMap.h"
#include "Timer.h"
#include "UIConfig.h"
#include "UIManager.h"

namespace game
{
class EditorStateUIConfigBuilder;

class EditorState : public State
{
public:
    friend class EditorStateUIConfigBuilder;

    EditorState(const std::shared_ptr<window::Window>&, const std::shared_ptr<graphics::RendererPool>&,
                std::shared_ptr<utils::FileAccess>, States&, std::shared_ptr<components::ui::UIManager>,
                std::shared_ptr<TileMap>, const std::shared_ptr<components::core::SharedContext>&,
                std::unique_ptr<ComponentOwnersManager>);

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
    const float timeAfterStateCouldBePaused;
    std::vector<std::shared_ptr<components::core::ComponentOwner>> clickableTileMap;
    std::shared_ptr<TileType> currentTileType;
    std::vector<LayoutTile> layoutTileMap;
    std::shared_ptr<TileMap> tileMap;
    std::shared_ptr<components::ui::UIManager> uiManager;
    const std::shared_ptr<components::core::SharedContext>& sharedContext;
    std::shared_ptr<components::core::ComponentOwner> camera;
    std::unique_ptr<ComponentOwnersManager> componentOwnersManager;
};
}