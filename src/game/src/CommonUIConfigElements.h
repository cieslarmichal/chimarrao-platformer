#pragma once

#include "ProjectPathReader.h"

namespace game
{
const auto projectPath = utils::ProjectPathReader::getProjectRootPath();
const auto fontPath = projectPath + "resources/fonts/VeraMono.ttf";
const auto menuBackgroundPath = projectPath + "resources/BG/menu_background.jpg";
const auto gameBackgroundPath = projectPath + "resources/BG/background_glacial_mountains.png";
const auto chooseMapBackgroundPath = projectPath + "resources/BG/wooden_background1.jpg";
const auto iconPath = projectPath + "resources/yerba_item.png";
}
