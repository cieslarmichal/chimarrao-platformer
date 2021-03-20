#pragma once

#include <memory>

namespace components::ui
{
struct UIConfig;
}

namespace game
{
class EditorMenuState;

class EditorMenuStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createEditorMenuUIConfig(EditorMenuState*);
};
}