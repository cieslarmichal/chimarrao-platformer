#include "Level1Controller.h"

#include <utility>

namespace game
{
namespace
{
const std::string mapsDirectory{utils::ProjectPathReader::getProjectRootPath() + "maps/story/"};
}

Level1Controller::Level1Controller(const std::shared_ptr<TileMap>& tileMap,
                                     std::shared_ptr<Level1MainCharacters> level1AMainCharacters)
    : mainCharacters{std::move(level1AMainCharacters)}
{
    const auto map = mapsDirectory + "level1a.map";
    tileMap->loadFromFile(map);
}

SwitchToNextLevel Level1Controller::update()
{
    return false;
}
}