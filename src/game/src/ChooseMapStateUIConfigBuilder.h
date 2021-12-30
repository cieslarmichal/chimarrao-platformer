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
    ChooseMapStateUIConfigBuilder();

    std::unique_ptr<components::ui::UIConfig> createChooseMapUIConfig(ChooseMapState*);
    std::vector<std::string> getNonNavigationButtonNames();
    std::vector<std::string> getIconNames();

private:
    std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(ChooseMapState*);
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(ChooseMapState*);
    std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs(ChooseMapState*);
    std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(ChooseMapState*);
    std::vector<std::unique_ptr<components::ui::TextFieldConfig>> createTextFieldConfigs(ChooseMapState*);
    std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(ChooseMapState*);

    const std::vector<std::string> allMapButtonsUniqueNames;
    std::vector<std::string> actualMapButtonsUniqueNames;
    const std::vector<std::string> allIconNames;
    std::vector<std::string> actualIconNames;
    std::size_t numberOfButtons;
};
}