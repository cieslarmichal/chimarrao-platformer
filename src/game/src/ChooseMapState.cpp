#include "ChooseMapState.h"

#include <cmath>

#include "ChooseMapStateUIConfigBuilder.h"

namespace game
{
ChooseMapState::ChooseMapState(const std::shared_ptr<window::Window>& windowInit,
                               const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                               std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                               std::unique_ptr<components::ui::UIManager> uiManagerInit,
                               std::unique_ptr<MapsReader> mapsReaderInit,
                               std::shared_ptr<TileMap> tileMapInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      uiManager{std::move(uiManagerInit)},
      mapsReader{std::move(mapsReaderInit)},
      shouldBackToMenu{false},
      mapsCurrentPage{1},
      tileMap{tileMapInit}
{
    mapFilePaths = mapsReader->readMapFilePaths();
    std::sort(mapFilePaths.begin(), mapFilePaths.end());
    std::transform(mapFilePaths.begin(), mapFilePaths.end(), std::back_inserter(mapNames),
                   [&](const std::string& mapFile)
                   { return fileAccess->getFileNameWithoutExtension(mapFile); });

    for (int mapIndex = 0; mapIndex < mapFilePaths.size() && mapIndex < maximumNumberOfMapsToDisplay;
         mapIndex++)
    {
        mapButtonsUniqueNames.push_back("chooseMap" + std::to_string(mapIndex + 1) + "MapButton");
    }

    mapsPages = static_cast<unsigned int>(
        std::ceil(static_cast<float>(mapNames.size()) / static_cast<float>(maximumNumberOfMapsToDisplay)));

    uiManager->createUI(ChooseMapStateUIConfigBuilder::createChooseMapUIConfig(this));
}

NextState ChooseMapState::update(const utils::DeltaTime& deltaTime, const input::Input& input)
{
    if (shouldBackToMenu)
    {
        return NextState::Menu;
    }

    uiManager->update(deltaTime, input);
    return NextState::Same;
}

void ChooseMapState::lateUpdate(const utils::DeltaTime&) {}

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
}

void ChooseMapState::deactivate()
{
    active = false;
    uiManager->deactivate();
}

void ChooseMapState::showNextMaps()
{
    if (mapsCurrentPage < mapsPages)
    {
        const auto numberOfMapsRemaining = mapNames.size() - mapsCurrentPage * maximumNumberOfMapsToDisplay;

        for (auto uniqueMapIndex = 0;
             uniqueMapIndex < numberOfMapsRemaining && uniqueMapIndex < maximumNumberOfMapsToDisplay;
             uniqueMapIndex++)
        {
            const auto currentMapNameIndex = mapsCurrentPage * maximumNumberOfMapsToDisplay + uniqueMapIndex;
            const auto& mapName = mapNames[currentMapNameIndex];
            const auto& mapPath = mapFilePaths[currentMapNameIndex];
            uiManager->setText(components::ui::UIComponentTypeWithLabel::Button,
                               mapButtonsUniqueNames[uniqueMapIndex], mapName);
            auto loadMap = [&]
            {
                tileMap->loadFromFile(mapPath);
                states.deactivateCurrentState();
                states.addNextState(StateType::Game);
            };
            components::core::KeyAction loadMapKeyAction{input::InputKey::MouseLeft, loadMap};
            uiManager->changeClickAction(components::ui::UIComponentType::Button,
                                         mapButtonsUniqueNames[uniqueMapIndex], {loadMapKeyAction});
        }

        if (numberOfMapsRemaining < maximumNumberOfMapsToDisplay)
        {
            for (auto mapIndex = numberOfMapsRemaining; mapIndex < maximumNumberOfMapsToDisplay; mapIndex++)
            {
                uiManager->deactivateComponent(components::ui::UIComponentType::Button,
                                               mapButtonsUniqueNames[mapIndex]);
            }
        }
        mapsCurrentPage++;
    }
}

void ChooseMapState::showPreviousMaps()
{
    if (mapsCurrentPage > 1)
    {
        for (auto mapIndex = 0; mapIndex < maximumNumberOfMapsToDisplay; mapIndex++)
        {
            const auto currentMapNameIndex = (mapsCurrentPage - 2) * maximumNumberOfMapsToDisplay + mapIndex;
            const auto& mapName = mapNames[currentMapNameIndex];
            const auto& mapPath = mapFilePaths[currentMapNameIndex];
            uiManager->activateComponent(components::ui::UIComponentType::Button,
                                         mapButtonsUniqueNames[mapIndex]);
            uiManager->setText(components::ui::UIComponentTypeWithLabel::Button,
                               mapButtonsUniqueNames[mapIndex], mapName);
            auto loadMap = [&]
            {
                tileMap->loadFromFile(mapPath);
                states.deactivateCurrentState();
                states.addNextState(StateType::Game);
            };
            components::core::KeyAction loadMapKeyAction{input::InputKey::MouseLeft, loadMap};
            uiManager->changeClickAction(components::ui::UIComponentType::Button,
                                         mapButtonsUniqueNames[mapIndex], {loadMapKeyAction});
        }

        mapsCurrentPage--;
    }
}
}