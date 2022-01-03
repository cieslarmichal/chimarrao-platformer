#include "Level1aController.h"

#include <utility>

namespace game
{
namespace
{
const std::string mapsDirectory{utils::ProjectPathReader::getProjectRootPath() + "maps/story/"};
}

Level1aController::Level1aController(const std::shared_ptr<TileMap>& tileMap,
                                     std::shared_ptr<Level1aMainCharacters> level1AMainCharacters)
    : mainCharacters{std::move(level1AMainCharacters)}
{
    const auto map = mapsDirectory + "level1a.map";
    tileMap->loadFromFile(map);
}

SwitchToNextLevel Level1aController::update()
{
    return false;
}
}