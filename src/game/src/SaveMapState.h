#pragma once

#include "State.h"
#include "Timer.h"
#include "editor/TileMap.h"
#include "ui/DefaultUIManager.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"

namespace game
{
class SaveMapStateUIConfigBuilder;

class SaveMapState : public State
{
    friend class SaveMapStateUIConfigBuilder;

public:
    explicit SaveMapState(const std::shared_ptr<window::Window>&,
                          const std::shared_ptr<graphics::RendererPool>&, States&,
                          std::unique_ptr<components::ui::UIManager>, TileMap&);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    void saveMap();

    utils::Timer possibleLeaveFromStateTimer;
    const float timeAfterLeaveStateIsPossible;
    bool shouldBackToEditorMenu;
    std::string currentMapName;
    std::unique_ptr<components::ui::UIManager> uiManager;
    TileMap& tileMap;
};
}