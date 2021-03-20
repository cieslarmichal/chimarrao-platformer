#pragma once

#include <memory>

namespace components::ui
{
struct UIConfig;
}

namespace game
{
class SaveMapState;

class SaveMapStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createSaveMapUIConfig(SaveMapState*);
};
}