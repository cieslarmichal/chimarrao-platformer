#pragma once

#include <editor/LayoutTile.h>
#include <vector>

#include "InputObserver.h"
#include "State.h"
#include "Timer.h"
#include "core/ClickableComponent.h"
#include "editor/TileMap.h"

namespace game
{
class EditorState : public State, public input::InputObserver
{
public:
    explicit EditorState(const std::shared_ptr<window::Window>&, const std::shared_ptr<input::InputManager>&,
                         const std::shared_ptr<graphics::RendererPool>&, std::stack<std::unique_ptr<State>>&);
    ~EditorState();

    void initialize();
    void update(const utils::DeltaTime&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    std::string getName() const override;
    void activate() override;
    void deactivate() override;
    void handleInputStatus(const input::InputStatus&) override;
    void pause();

private:
    void unfreezeButtons();

    const input::InputStatus* inputStatus;
    bool paused;
    utils::Timer pauseTimer;
    utils::Timer moveTimer;
    const float timeAfterStateCouldBePaused;
    const float timeBetweenTileMoves;
    int currentTileId;
    std::string currentTilePath;
    std::unique_ptr<components::core::ComponentOwner> background;
    std::vector<std::shared_ptr<components::core::ComponentOwner>> clickableTileMap;
    std::shared_ptr<TileType> currentTileType;
    std::vector<LayoutTile> layoutTileMap;
    std::unique_ptr<TileMap> tileMap;
    bool buttonsActionsFrozen = true;
    utils::Timer freezeClickableButtonsTimer;
    const float timeAfterButtonsCanBeClicked;
};
}