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
struct ImageConfig;
}

namespace game
{
class EditorMenuState;

class EditorMenuStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createEditorMenuUIConfig(EditorMenuState*);

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(EditorMenuState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(EditorMenuState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
    createCheckBoxConfigs(EditorMenuState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(EditorMenuState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
    createTextFieldConfigs(EditorMenuState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(EditorMenuState*);
};
}