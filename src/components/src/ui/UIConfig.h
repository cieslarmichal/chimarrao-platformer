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
    UIConfig(std::unique_ptr<BackgroundConfig> backgroundConfigInit,
             std::vector<std::unique_ptr<ButtonConfig>> buttonsConfigInit,
             std::vector<std::unique_ptr<CheckBoxConfig>> checkBoxesConfigInit,
             std::vector<std::unique_ptr<LabelConfig>> labelsConfigInit,
             std::vector<std::unique_ptr<TextFieldConfig>> textFieldsConfigInit)
        : backgroundConfig{std::move(backgroundConfigInit)},
          buttonsConfig{std::move(buttonsConfigInit)},
          checkBoxesConfig{std::move(checkBoxesConfigInit)},
          labelsConfig{std::move(labelsConfigInit)},
          textFieldsConfig{std::move(textFieldsConfigInit)}
    {
    }

    std::unique_ptr<BackgroundConfig> backgroundConfig;
    std::vector<std::unique_ptr<ButtonConfig>> buttonsConfig;
    std::vector<std::unique_ptr<CheckBoxConfig>> checkBoxesConfig;
    std::vector<std::unique_ptr<LabelConfig>> labelsConfig;
    std::vector<std::unique_ptr<TextFieldConfig>> textFieldsConfig;
};
}