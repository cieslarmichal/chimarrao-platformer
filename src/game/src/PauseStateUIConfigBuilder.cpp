#include "PauseStateUIConfigBuilder.h"

#include "CommonUIConfigElements.h"
#include "PauseState.h"
#include "ProjectPathReader.h"

namespace game
{

namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
const auto textColor = graphics::Color(200, 200, 200);
const auto buttonSize = utils::Vector2f{25, 5};
}

std::unique_ptr<components::ui::UIConfig>
PauseStateUIConfigBuilder::createPauseUIConfig(PauseState* pauseState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(pauseState), std::move(createButtonConfigs(pauseState)),
        createCheckBoxConfigs(pauseState), createLabelConfigs(pauseState), createTextFieldConfigs(pauseState),
        createImageConfigs(pauseState));
}

std::unique_ptr<components::ui::BackgroundConfig>
PauseStateUIConfigBuilder::createBackgroundConfig(PauseState*)
{
    return std::make_unique<components::ui::BackgroundConfig>(
        "pauseBackground", utils::Vector2f{25, 10}, utils::Vector2f{31, 32},
        graphics::VisibilityLayer::Second, graphics::Color{172});
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
PauseStateUIConfigBuilder::createButtonConfigs(PauseState* pauseState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;

    const auto backToGameButtonPosition = utils::Vector2f{28, 21};
    const auto backToGameButtonOnMouseOver = [=]
    { pauseState->uiManager->setColor("pauseBackToGameButton", buttonHoverColor); };
    const auto backToGameButtonOnMouseOut = [=]
    { pauseState->uiManager->setColor("pauseBackToGameButton", buttonColor); };
    auto backToGameButtonMouseOverActions =
        components::ui::MouseOverActions{backToGameButtonOnMouseOver, backToGameButtonOnMouseOut};
    auto backToGameClickAction = [=] { pauseState->shouldBackToGame = true; };
    auto backToGameButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "pauseBackToGameButton", backToGameButtonPosition, buttonSize, buttonColor, "Back to game", textColor,
        30, fontPath, utils::Vector2f{2, 0.5}, backToGameClickAction, backToGameButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToGameButtonConfig));

    const auto backToMenuButtonPosition = utils::Vector2f{28, 30};
    const auto backToMenuButtonOnMouseOver = [=]
    { pauseState->uiManager->setColor("pauseBackToMenuButton", buttonHoverColor); };
    const auto backToMenuButtonOnMouseOut = [=]
    { pauseState->uiManager->setColor("pauseBackToMenuButton", buttonColor); };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [=] { pauseState->shouldBackToMenu = true; };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "pauseBackToMenuButton", backToMenuButtonPosition, buttonSize, buttonColor, "Back to menu", textColor,
        30, fontPath, utils::Vector2f{2, 0.5}, backToMenuClickAction, backToMenuButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToMenuButtonConfig));

    return buttonsConfig;
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
PauseStateUIConfigBuilder::createCheckBoxConfigs(PauseState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::LabelConfig>>
PauseStateUIConfigBuilder::createLabelConfigs(PauseState*)
{
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;

    const auto textPausePosition = utils::Vector2f{35, 13};
    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "pauseTitleLabel", textPausePosition, graphics::Color::White, "Pause", 40, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

    return labelsConfig;
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
PauseStateUIConfigBuilder::createTextFieldConfigs(PauseState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::ImageConfig>>
PauseStateUIConfigBuilder::createImageConfigs(PauseState*)
{
    return {};
}
}