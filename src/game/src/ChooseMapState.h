#pragma once

#include "ComponentOwner.h"
#include "MapsReader.h"
#include "PaginatedButtonsNavigator.h"
#include "State.h"
#include "Timer.h"
#include "UIConfig.h"
#include "UIManager.h"
#include "editor/TileMap.h"

namespace game
{
class ChooseMapStateUIConfigBuilder;

class ChooseMapState : public State
{
    friend class ChooseMapStateUIConfigBuilder;

public:
    explicit ChooseMapState(const std::shared_ptr<window::Window>&,
                            const std::shared_ptr<graphics::RendererPool>&,
                            std::shared_ptr<utils::FileAccess>, States&,
                            std::shared_ptr<components::ui::UIManager>, std::shared_ptr<TileMap>,
                            std::unique_ptr<MapsReader>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    void backToMenu();

    std::shared_ptr<components::ui::UIManager> uiManager;
    bool shouldBackToMenu;
    std::shared_ptr<TileMap> tileMap;
    std::unique_ptr<MapsReader> mapsReader;
    std::vector<std::string> mapFilePaths;
    std::vector<std::string> mapNames;
    std::function<void(int)> paginatedButtonActionForButtonIndex;
    std::unique_ptr<PaginatedButtonsNavigator> buttonsNavigator;
};
}