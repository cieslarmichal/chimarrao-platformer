#pragma once

#include <memory>
#include "SettingsState.h"


namespace components::ui
{
struct UIConfig;
}

namespace game
{
class SettingsState;

class SettingsStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createSettingsUIConfig(SettingsState*);
};
}