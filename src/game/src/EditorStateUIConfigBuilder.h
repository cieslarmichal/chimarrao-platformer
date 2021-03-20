#pragma once

#include <memory>

namespace components::ui
{
struct UIConfig;
}

namespace game
{
class EditorState;

class EditorStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createEditorUIConfig(EditorState*);
};
}