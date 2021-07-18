#include "HeadsUpDisplayUIConfigBuilder.h"

#include "CommonUIConfigElements.h"
#include "ui/UIConfig.h"

namespace game
{

std::unique_ptr<components::ui::UIConfig> HeadsUpDisplayUIConfigBuilder::createUIConfig()
{
    return std::make_unique<components::ui::UIConfig>(createBackgroundConfig(), createButtonConfigs(),
                                                      createCheckBoxConfigs(), createLabelConfigs(),
                                                      createTextFieldConfigs(), createImageConfigs());
}

std::string HeadsUpDisplayUIConfigBuilder::getHealthBarId()
{
    return "hudHealthPointsBar";
}

std::unique_ptr<components::ui::BackgroundConfig> HeadsUpDisplayUIConfigBuilder::createBackgroundConfig()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
HeadsUpDisplayUIConfigBuilder::createButtonConfigs()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
HeadsUpDisplayUIConfigBuilder::createCheckBoxConfigs()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::LabelConfig>> HeadsUpDisplayUIConfigBuilder::createLabelConfigs()
{
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;

    const auto healthPointsLabelPosition = utils::Vector2f{1, 1};
    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "hudHealthPointsLabel", healthPointsLabelPosition, graphics::Color::Red, "HP", 30, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

    return labelsConfig;
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
HeadsUpDisplayUIConfigBuilder::createTextFieldConfigs()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::ImageConfig>> HeadsUpDisplayUIConfigBuilder::createImageConfigs()
{
    std::vector<std::unique_ptr<components::ui::ImageConfig>> imagesConfig;

    const auto healthPointsBarPosition = utils::Vector2f{1, 10};
    const auto healthPointsBarSize = utils::Vector2f{5, 1};
    auto imageConfig = std::make_unique<components::ui::ImageConfig>(
        "hudHealthPointsBar", healthPointsBarPosition, healthPointsBarSize, graphics::VisibilityLayer::First,
        graphics::Color::Red);
    imagesConfig.push_back(std::move(imageConfig));

    return imagesConfig;
}
}