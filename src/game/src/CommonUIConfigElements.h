#pragma once

#include "GetProjectPath.h"

namespace game
{
const auto projectPath = utils::getProjectPath("chimarrao-platformer");
const auto fontPath = projectPath + "resources/fonts/VeraMono.ttf";
const auto menuBackgroundPath = projectPath + "resources/BG/menu_background.jpg";
const auto gameBackgroundPath = projectPath + "resources/BG/background_glacial_mountains.png";
const auto iconPath = projectPath + "resources/yerba_item.png";

}
