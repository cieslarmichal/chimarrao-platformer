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
class ChooseMapState;

class ChooseMapStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createChooseMapUIConfig(ChooseMapState*);

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(ChooseMapState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(ChooseMapState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
    createCheckBoxConfigs(ChooseMapState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(ChooseMapState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
    createTextFieldConfigs(ChooseMapState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(ChooseMapState*);
};
}