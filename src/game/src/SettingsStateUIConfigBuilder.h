#pragma once

#include <memory>
#include <vector>

#include "SettingsState.h"

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
class SettingsState;

class SettingsStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createSettingsUIConfig(SettingsState*);

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(SettingsState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(SettingsState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs(SettingsState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(SettingsState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
    createTextFieldConfigs(SettingsState*);
};
}