#pragma once

#include <memory>
#include <vector>

#include "GridButtonInfo.h"

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
class ControlsState;

class ControlsStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createControlsUIConfig(ControlsState*);
    static std::vector<std::vector<GridButtonInfo>> getGridButtonsInfo();
    static std::vector<std::string> getIconNames();

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(ControlsState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(ControlsState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs(ControlsState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(ControlsState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
    createTextFieldConfigs(ControlsState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(ControlsState*);

    static std::vector<std::string> iconNames;
};
}