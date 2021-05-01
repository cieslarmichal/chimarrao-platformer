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
class SaveMapState;

class SaveMapStateUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createSaveMapUIConfig(SaveMapState*);

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig(SaveMapState*);
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs(SaveMapState*);
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs(SaveMapState*);
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs(SaveMapState*);
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
    createTextFieldConfigs(SaveMapState*);
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs(SaveMapState*);
};
}