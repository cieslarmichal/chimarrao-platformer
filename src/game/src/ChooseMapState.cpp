#include "ChooseMapState.h"

#include <cmath>

#include "ChooseMapStateUIConfigBuilder.h"

namespace game
{
ChooseMapState::ChooseMapState(const std::shared_ptr<window::Window>& windowInit,
                               const std::shared_ptr<graphics::RendererPool>& rendererPoolInit,
                               std::shared_ptr<utils::FileAccess> fileAccessInit, States& statesInit,
                               std::unique_ptr<components::ui::UIManager> uiManagerInit)
    : State{windowInit, rendererPoolInit, std::move(fileAccessInit), statesInit},
      uiManager{std::move(uiManagerInit)},
      mapFilePathsReader{std::make_unique<MapFilePathsReader>(fileAccess)},
      shouldBackToMenu{false},
      mapsCurrentPage{1}
{
    mapFiles = mapFilePathsReader->readMapFilePaths();
    std::transform(mapFiles.begin(), mapFiles.end(), std::back_inserter(mapNames),
                   [&](const std::string& mapFile)
                   { return fileAccess->getFileNameWithoutExtension(mapFile); });

    for (int mapIndex = 0; mapIndex < mapFiles.size() && mapIndex < maximumNumberOfMapsToDisplay; mapIndex++)
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

        for (auto mapIndex = 0; mapIndex < numberOfMapsRemaining && mapIndex < maximumNumberOfMapsToDisplay; mapIndex++)
        {
            uiManager->setText(components::ui::UIComponentTypeWithLabel::Button, mapButtonsUniqueNames[mapIndex], mapNames[mapsCurrentPage*maximumNumberOfMapsToDisplay + mapIndex]);
        }

        if (numberOfMapsRemaining < maximumNumberOfMapsToDisplay)
        {
            for (auto mapIndex = numberOfMapsRemaining; mapIndex< maximumNumberOfMapsToDisplay; mapIndex++)
            {
                uiManager->deactivateComponent(components::ui::UIComponentType::Button, mapButtonsUniqueNames[mapIndex]);
            }
        }
            mapsCurrentPage++;
    }
}

void ChooseMapState::showPreviousMaps() {}
}