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

std::string HeadsUpDisplayUIConfigBuilder::getHealthBarFrameId()
{
    return "hudHealthPointsBarFrame";
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

    const auto healthPointsLabelPosition = utils::Vector2f{0.5, 0.5};
    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "hudHealthPointsLabel", healthPointsLabelPosition, graphics::Color::Red, "HP", 20, fontPath);
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

    const auto healthPointsBarPosition = utils::Vector2f{4, 1.2};
    const auto healthPointsBarSize = utils::Vector2f{7, 1.3};
    auto healthBarImageConfig = std::make_unique<components::ui::ImageConfig>(
        "hudHealthPointsBar", healthPointsBarPosition, healthPointsBarSize, graphics::VisibilityLayer::First,
        graphics::Color::Red);
    imagesConfig.push_back(std::move(healthBarImageConfig));

    auto healthBarFrameImageConfig = std::make_unique<components::ui::ImageConfig>(
        "hudHealthPointsBarFrame", healthPointsBarPosition, healthPointsBarSize,
        graphics::VisibilityLayer::First, graphics::Color::Transparent);
    imagesConfig.push_back(std::move(healthBarFrameImageConfig));

    return imagesConfig;
}
}