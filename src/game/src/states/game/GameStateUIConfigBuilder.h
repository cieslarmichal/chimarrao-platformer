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
class GameStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createGameUIConfig();

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig();
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs();
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs();
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs();
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>> createTextFieldConfigs();
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs();
};
}