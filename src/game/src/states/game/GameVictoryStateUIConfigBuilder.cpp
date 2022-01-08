#include "GameVictoryStateUIConfigBuilder.h"

#include "CommonUIConfigElements.h"
#include "GameVictoryState.h"
#include "ProjectPathReader.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color{65, 105, 200};
const auto buttonHoverColor = graphics::Color(4, 8, 97);
const auto textColor = graphics::Color(200, 200, 200);
const auto buttonSize = utils::Vector2f{25, 5};
const auto backToMenuButtonPosition = utils::Vector2f{28, 30};
const std::vector<utils::Vector2f> iconsPositions{backToMenuButtonPosition};
const auto iconSize = utils::Vector2f{4, 4};
}

std::vector<std::string> GameVictoryStateUIConfigBuilder::iconNames{"gameVictoryIcon1Image"};

std::unique_ptr<components::ui::UIConfig>
GameVictoryStateUIConfigBuilder::createGameVictoryUIConfig(GameVictoryState* gameVictoryState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(gameVictoryState), createButtonConfigs(gameVictoryState),
        createCheckBoxConfigs(gameVictoryState), createLabelConfigs(gameVictoryState),
        createTextFieldConfigs(gameVictoryState), createImageConfigs(gameVictoryState));
}

std::unique_ptr<components::ui::BackgroundConfig>
GameVictoryStateUIConfigBuilder::createBackgroundConfig(GameVictoryState*)
{
    return std::make_unique<components::ui::BackgroundConfig>(
        "gameVictoryBackground", utils::Vector2f{22, 10}, utils::Vector2f{37, 32},
        graphics::VisibilityLayer::Second, graphics::Color{172});
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
GameVictoryStateUIConfigBuilder::createButtonConfigs(GameVictoryState* gameVictoryState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;

    const auto backToMenuButtonOnMouseOver = [=]
    { gameVictoryState->uiManager->setColor("gameVictoryBackToMenuButton", buttonHoverColor); };
    const auto backToMenuButtonOnMouseOut = [=]
    { gameVictoryState->uiManager->setColor("gameVictoryBackToMenuButton", buttonColor); };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [=] { gameVictoryState->backToMenu(); };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "gameVictoryBackToMenuButton", backToMenuButtonPosition, buttonSize, buttonColor, "Back to menu",
        textColor, 30, fontPath, utils::Vector2f{2.f, 0.5}, backToMenuClickAction,
        backToMenuButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToMenuButtonConfig));

    return buttonsConfig;
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
GameVictoryStateUIConfigBuilder::createCheckBoxConfigs(GameVictoryState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::LabelConfig>>
GameVictoryStateUIConfigBuilder::createLabelConfigs(GameVictoryState*)
{
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;

    const auto textGameVictoryPosition = utils::Vector2f{30, 13};
    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "gameVictoryTitleLabel", textGameVictoryPosition, graphics::Color::White, "You won!", 40, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

    return labelsConfig;
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
GameVictoryStateUIConfigBuilder::createTextFieldConfigs(GameVictoryState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::ImageConfig>>
GameVictoryStateUIConfigBuilder::createImageConfigs(GameVictoryState*)
{
    std::vector<std::unique_ptr<components::ui::ImageConfig>> imagesConfig;

    for (std::size_t iconIndex = 0; iconIndex < iconNames.size(); iconIndex++)
    {
        const auto iconPosition = utils::Vector2f{iconsPositions[iconIndex].x - iconSize.x - 0.25f,
                                                  iconsPositions[iconIndex].y + 0.4f};
        auto imageConfig = std::make_unique<components::ui::ImageConfig>(
            iconNames[iconIndex], iconPosition, iconSize, graphics::VisibilityLayer::First, iconPath);
        imagesConfig.push_back(std::move(imageConfig));
    }

    return imagesConfig;
}

}