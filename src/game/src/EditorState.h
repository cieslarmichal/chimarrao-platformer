#pragma once

#include <vector>

#include "State.h"
#include "Timer.h"
#include "editor/LayoutTile.h"
#include "editor/TileMap.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"

namespace game
{
class EditorStateUIConfigBuilder;

class EditorState : public State
{
public:
    friend class EditorStateUIConfigBuilder;

    explicit EditorState(const std::shared_ptr<window::Window>&,
                         const std::shared_ptr<graphics::RendererPool>&, std::stack<std::unique_ptr<State>>&,
                         std::unique_ptr<components::ui::UIManager>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void pause();

private:
    bool paused;
    utils::Timer pauseTimer;
    utils::Timer moveTimer;
    const float timeAfterStateCouldBePaused;
    const float timeBetweenTileMoves;
    int currentTileId;
    std::string currentTilePath;
    std::vector<std::shared_ptr<components::core::ComponentOwner>> clickableTileMap;
    std::shared_ptr<TileType> currentTileType;
    std::vector<LayoutTile> layoutTileMap;
    std::unique_ptr<TileMap> tileMap;
    bool buttonsActionsFrozen = true;
    utils::Timer freezeClickableButtonsTimer;
    const float timeAfterButtonsCanBeClicked;
    std::unique_ptr<components::ui::UIManager> uiManager;
};
}