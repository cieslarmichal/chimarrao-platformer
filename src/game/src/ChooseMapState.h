#pragma once

#include "MapsReader.h"
#include "State.h"
#include "Timer.h"
#include "core/ComponentOwner.h"
#include "editor/TileMap.h"
#include "ui/UIConfig.h"
#include "ui/UIManager.h"

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
                            std::unique_ptr<components::ui::UIManager>, std::unique_ptr<MapsReader>,
                            std::shared_ptr<TileMap>);

    NextState update(const utils::DeltaTime&, const input::Input&) override;
    void lateUpdate(const utils::DeltaTime&, const input::Input&) override;
    void render() override;
    StateType getType() const override;
    void activate() override;
    void deactivate() override;

private:
    void showNextMaps();
    void showPreviousMaps();

    std::unique_ptr<components::ui::UIManager> uiManager;
    std::unique_ptr<MapsReader> mapsReader;
    bool shouldBackToMenu;
    std::vector<std::string> mapFilePaths;
    std::vector<std::string> mapNames;
    unsigned int mapsCurrentPage;
    unsigned int mapsPages;
    const int maximumNumberOfMapsToDisplay = 5;
    std::vector<std::string> mapButtonsUniqueNames;
    std::shared_ptr<TileMap> tileMap;
};
}