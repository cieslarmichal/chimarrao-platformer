#pragma once

#include <memory>
#include <string>
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
class MenuState;

class MenuStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createMenuUIConfig(MenuState*);
    static std::vector<std::vector<GridButtonInfo>> getGridButtonsInfo();
    static std::vector<std::string> getIconNames();

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(MenuState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(MenuState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs(MenuState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(MenuState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>> createTextFieldConfigs(MenuState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(MenuState*);

    static std::vector<std::string> buttonNames;
    static std::vector<std::string> iconNames;
};
}