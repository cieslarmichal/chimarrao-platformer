#pragma once

#include <vector>

#include "InputObserver.h"
#include "State.h"
#include "Timer.h"
#include "core/ClickableComponent.h"
#include "editor/LayoutTile.h"
#include "editor/TileMap.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"

namespace game
{
class EditorStateUIConfigBuilder;

class EditorState : public State, public input::InputObserver
{
public:
    friend class EditorStateUIConfigBuilder;

    explicit EditorState(const std::shared_ptr<window::Window>&, const std::shared_ptr<input::InputManager>&,
                         const std::shared_ptr<graphics::RendererPool>&,
                         const std::shared_ptr<utils::FileAccess>&, std::stack<std::unique_ptr<State>>&,
                         std::unique_ptr<components::ui::UIManager>);
    ~EditorState();

    NextState update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;
    void pause();

private:
    const input::InputStatus* inputStatus;
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