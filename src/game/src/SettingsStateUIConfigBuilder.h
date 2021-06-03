#pragma once

#include <memory>
#include <vector>

#include "GridButtonInfo.h"
#include "SettingsState.h"

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
class SettingsState;

class SettingsStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createSettingsUIConfig(SettingsState*);
    static std::vector<std::vector<GridButtonInfo>> getGridButtonsInfo();
    static std::vector<std::string> getIconNames();

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(SettingsState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(SettingsState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs(SettingsState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(SettingsState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
    createTextFieldConfigs(SettingsState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(SettingsState*);

    static std::vector<std::string> iconNames;
};
}