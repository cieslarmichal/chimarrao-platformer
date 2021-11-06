#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ui/UIConfig.h"

namespace components::ui
{
struct BackgroundConfig;
struct ButtonConfig;
struct CheckBoxConfig;
struct LabelConfig;
struct TextFieldConfig;
struct ImageConfig;
}

namespace game
{
class HeadsUpDisplayUIConfigBuilder
{
public:
    static std::unique_ptr<components::ui::UIConfig> createUIConfig();
    static std::string getHealthBarId();
    static std::string getHealthBarFrameId();
    static std::vector<std::string> getSlotIds();
    static std::vector<std::string> getSlotItemIds();

private:
    static std::unique_ptr<components::ui::BackgroundConfig> createBackgroundConfig();
    static std::vector<std::unique_ptr<components::ui::ButtonConfig>> createButtonConfigs();
    static std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> createCheckBoxConfigs();
    static std::vector<std::unique_ptr<components::ui::LabelConfig>> createLabelConfigs();
    static std::vector<std::unique_ptr<components::ui::TextFieldConfig>> createTextFieldConfigs();
    static std::vector<std::unique_ptr<components::ui::ImageConfig>> createImageConfigs();

    static std::size_t numberOfSlots;
};
}