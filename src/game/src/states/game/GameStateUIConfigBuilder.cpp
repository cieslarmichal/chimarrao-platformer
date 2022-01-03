#include "GameStateUIConfigBuilder.h"

#include "CommonUIConfigElements.h"
#include "CustomGameState.h"

namespace game
{

std::unique_ptr<components::ui::UIConfig> GameStateUIConfigBuilder::createGameUIConfig()
{
    return std::make_unique<components::ui::UIConfig>(createBackgroundConfig(), createButtonConfigs(),
                                                      createCheckBoxConfigs(), createLabelConfigs(),
                                                      createTextFieldConfigs(), createImageConfigs());
}

std::unique_ptr<components::ui::BackgroundConfig> GameStateUIConfigBuilder::createBackgroundConfig()
{
    return std::make_unique<components::ui::BackgroundConfig>(
        "gameBackground", utils::Vector2f{0, 0}, utils::Vector2f{80, 60},
        graphics::VisibilityLayer::Background, gameBackgroundPath);
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>> GameStateUIConfigBuilder::createButtonConfigs()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>> GameStateUIConfigBuilder::createCheckBoxConfigs()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::LabelConfig>> GameStateUIConfigBuilder::createLabelConfigs()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
GameStateUIConfigBuilder::createTextFieldConfigs()
{
    return {};
}

std::vector<std::unique_ptr<components::ui::ImageConfig>> GameStateUIConfigBuilder::createImageConfigs()
{
    return {};
}

}