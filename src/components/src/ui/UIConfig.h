#pragma once

#include <memory>
#include <vector>

#include "BackgroundConfig.h"
#include "ButtonConfig.h"
#include "CheckBoxConfig.h"
#include "LabelConfig.h"
#include "TextFieldConfig.h"

namespace components::ui
{
struct UIConfig
{
    std::unique_ptr<BackgroundConfig> backgroundConfig;
    std::vector<std::unique_ptr<ButtonConfig>> buttonsConfig;
    std::vector<std::unique_ptr<CheckBoxConfig>> checkBoxesConfig;
    std::vector<std::unique_ptr<LabelConfig>> labelsConfig;
    std::vector<std::unique_ptr<TextFieldConfig>> textFieldsConfig;
};
}