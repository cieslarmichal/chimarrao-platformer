#include "ChooseMapState.h"

#include <utility>

#include "ChooseMapStateUIConfigBuilder.h"
#include "TimerFactory.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
}

ChooseMapState::ChooseMapState(const std::shared_ptr<window::Window>& windowInit,
                               const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                               std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                               std::shared_ptr<components::ui::UIManager> uiManagerInit,
                               std::shared_ptr<TileMap> tileMapInit, std::unique_ptr<MapsReader> mapsReader)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      uiManager{std::move(uiManagerInit)},
      shouldBackToMenu{false},
      tileMap{std::move(tileMapInit)},
      mapsReader{std::move(mapsReader)},
      mapFilePaths{this->mapsReader->readMapFilePaths()},
      mapNames{this->mapsReader->readMapNames()},
      paginatedButtonActionForButtonIndex{[&](int index)
                                          {
                                              const auto& mapPath = mapFilePaths[index];
                                              tileMap->loadFromFile(mapPath);
                                              states.deactivateCurrentState();
                                              states.addNextState(StateType::CustomGame);
                                          }}
{
    ChooseMapStateUIConfigBuilder uiConfigBuilder;

    this->uiManager->createUI(uiConfigBuilder.createChooseMapUIConfig(this));

    buttonsNavigator = std::make_unique<PaginatedButtonsNavigator>(
        uiManager, uiConfigBuilder.getNonNavigationButtonNames(), uiConfigBuilder.getIconNames(), mapNames,
        paginatedButtonActionForButtonIndex, 5, buttonColor, buttonHoverColor,
        utils::TimerFactory::createTimer(), utils::TimerFactory::createTimer());

    buttonsNavigator->initialize();
}

NextState ChooseMapState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (const auto nextState = buttonsNavigator->update(deltaTime, input); nextState == NextState::Previous)
    {
        return NextState::Previous;
    }

    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void ChooseMapState::lateUpdate(const utils::DeltaTime&, const input::Input&) {}

void ChooseMapState::render()
{
    rendererPool->renderAll();
}

StateType ChooseMapState::getType() const
{
    return StateType::ChooseMap;
}

void ChooseMapState::activate()
{
    active = true;
    uiManager->activate();
    buttonsNavigator->activate();
}

void ChooseMapState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void ChooseMapState::backToMenu()
{
    shouldBackToMenu = true;
}

}