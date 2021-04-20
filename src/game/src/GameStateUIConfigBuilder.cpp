#include "GameStateUIConfigBuilder.h"

#include "CommonUIConfigElements.h"
#include "GameState.h"
#include "ProjectPathReader.h"
#include "ui/DefaultUIManager.h"

namespace game
{

std::unique_ptr<components::ui::UIConfig> GameStateUIConfigBuilder::createGameUIConfig(GameState* gameState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(gameState), std::move(createButtonConfigs(gameState)),
        createCheckBoxConfigs(gameState), createLabelConfigs(gameState), createTextFieldConfigs(gameState));
}

std::unique_ptr<components::ui::BackgroundConfig> GameStateUIConfigBuilder::createBackgroundConfig(GameState*)
{
    return std::make_unique<components::ui::BackgroundConfig>(
        "gameBackground", utils::Vector2f{0, 0}, utils::Vector2f{80, 60},
        graphics::VisibilityLayer::Background, gameBackgroundPath);
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
GameStateUIConfigBuilder::createButtonConfigs(GameState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
GameStateUIConfigBuilder::createCheckBoxConfigs(GameState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::LabelConfig>>
GameStateUIConfigBuilder::createLabelConfigs(GameState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
GameStateUIConfigBuilder::createTextFieldConfigs(GameState*)
{
    return {};
}

}