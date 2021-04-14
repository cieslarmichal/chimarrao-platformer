#pragma once

#include <memory>
#include <vector>

namespace components::ui
{
struct UIConfig;
struct BackgroundConfig;
struct ButtonConfig;
struct CheckBoxConfig;
struct LabelConfig;
struct TextFieldConfig;
}

namespace game
{
class EditorState;

class EditorStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createEditorUIConfig(EditorState*);

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(EditorState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(EditorState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs(EditorState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(EditorState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>> createTextFieldConfigs(EditorState*);
};
}