#pragma once

#include <memory>
#include <string>
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
    static std::vector<std::string> getNonNavigationButtonNames();
    static std::vector<std::string> getIconNames();

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(ChooseMapState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(ChooseMapState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
    createCheckBoxConfigs(ChooseMapState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(ChooseMapState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
    createTextFieldConfigs(ChooseMapState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(ChooseMapState*);

    static const std::vector<std::string> mapButtonsUniqueNames;
    static const std::vector<std::string> iconNames;
};
}