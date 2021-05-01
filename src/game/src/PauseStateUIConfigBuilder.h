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
class PauseState;

class PauseStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createPauseUIConfig(PauseState*);

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(PauseState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(PauseState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs(PauseState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(PauseState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>> createTextFieldConfigs(PauseState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(PauseState*);
};
}