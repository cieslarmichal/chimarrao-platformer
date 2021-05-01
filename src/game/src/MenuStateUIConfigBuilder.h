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
class MenuState;

class MenuStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createMenuUIConfig(MenuState*);

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(MenuState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(MenuState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs(MenuState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(MenuState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>> createTextFieldConfigs(MenuState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(MenuState*);
};
}