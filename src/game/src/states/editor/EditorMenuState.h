#pragma once

#include "ComponentOwner.h"
#include "State.h"
#include "TileMap.h"
#include "Timer.h"
#include "UIConfig.h"
#include "UIManager.h"
#include "navigators/GridButtonsNavigator.h"

namespace game
{
class EditorMenuStateUIConfigBuilder;

class EditorMenuState : public State
{
public:
    friend class EditorMenuStateUIConfigBuilder;

    explicit EditorMenuState(const std::shared_ptr<window::Window>&,
                             const std::shared_ptr<graphics::RendererPool>&,
                             std::shared_ptr<utils::FileAccess>, States&,
                             std::shared_ptr<components::ui::UIManager>, std::shared_ptr<TileMap>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    void backToEditor();
    void backToMenu();
    void saveMap();
    void createNewMap();
    void loadMap();

    bool shouldBackToEditor;
    bool shouldBackToMenu;
    std::shared_ptr<components::ui::UIManager> uiManager;
    std::shared_ptr<TileMap> tileMap;
    std::unique_ptr<GridButtonsNavigator> buttonsNavigator;
};
}