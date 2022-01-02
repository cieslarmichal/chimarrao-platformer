#pragma once

#include "DefaultUIManager.h"
#include "navigators/GridButtonsNavigator.h"
#include "states/State.h"
#include "Timer.h"
#include "UIConfig.h"
#include "UIManager.h"
#include "TileMap.h"

namespace game
{
class SaveMapStateUIConfigBuilder;

class SaveMapState : public State
{
    friend class SaveMapStateUIConfigBuilder;

public:
    explicit SaveMapState(const std::shared_ptr<window::Window>&,
                          const std::shared_ptr<graphics::RendererPool>&, std::shared_ptr<utils::FileAccess>,
                          States&, std::shared_ptr<components::ui::UIManager>, std::shared_ptr<TileMap>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    void backToEditorMenu();
    void saveMap();

    bool shouldBackToEditorMenu;
    std::string currentMapName;
    std::shared_ptr<components::ui::UIManager> uiManager;
    std::shared_ptr<TileMap> tileMap;
    std::unique_ptr<GridButtonsNavigator> buttonsNavigator;
};
}