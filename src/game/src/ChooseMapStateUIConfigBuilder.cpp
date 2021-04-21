#include "ChooseMapStateUIConfigBuilder.h"

#include "ChooseMapState.h"
#include "Color.h"
#include "CommonUIConfigElements.h"
#include "ProjectPathReader.h"
#include "Vector.h"

namespace game
{
namespace
{
const auto buttonColor = graphics::Color(251, 190, 102);
const auto buttonHoverColor = graphics::Color(205, 128, 66);
const auto titlePosition = utils::Vector2f{30.f, 6.f};
const auto backToMenuButtonPosition = utils::Vector2f{34.5f, 48.f};
const auto leftArrowButtonPosition = utils::Vector2f{21.f, 27.5f};
const auto rightArrowButtonPosition = utils::Vector2f{53.f, 27.5f};
const auto arrowButtonSize = utils::Vector2f{8.f, 3.f};
const auto arrowButtonFontSize{20};
const auto firstMapButtonPosition = utils::Vector2f{33.5f, 15.f};
const auto mapButtonSize = utils::Vector2f{15.f, 4.f};
}

std::unique_ptr<components::ui::UIConfig>
ChooseMapStateUIConfigBuilder::createChooseMapUIConfig(ChooseMapState* chooseMapState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(chooseMapState), std::move(createButtonConfigs(chooseMapState)),
        createCheckBoxConfigs(chooseMapState), createLabelConfigs(chooseMapState),
        createTextFieldConfigs(chooseMapState));
}

std::unique_ptr<components::ui::BackgroundConfig>
ChooseMapStateUIConfigBuilder::createBackgroundConfig(ChooseMapState*)
{
    return std::make_unique<components::ui::BackgroundConfig>(
        "chooseMapBackground", utils::Vector2f{0.f, 0.f}, utils::Vector2f{80.f, 60.f},
        graphics::VisibilityLayer::Background, chooseMapBackgroundPath);
}

std::vector<std::unique_ptr<components::ui::ButtonConfig>>
ChooseMapStateUIConfigBuilder::createButtonConfigs(ChooseMapState* chooseMapState)
{
    std::vector<std::unique_ptr<components::ui::ButtonConfig>> buttonsConfig;

    const auto backToMenuButtonSize = utils::Vector2f{13.f, 5.f};
    const auto backToMenuButtonOnMouseOver = [=]
    {
        chooseMapState->uiManager->setColor(components::ui::UIComponentType::Button,
                                            "chooseMapBackToMenuButton", buttonHoverColor);
    };
    const auto backToMenuButtonOnMouseOut = [=]
    {
        chooseMapState->uiManager->setColor(components::ui::UIComponentType::Button,
                                            "chooseMapBackToMenuButton", buttonColor);
    };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [=] { chooseMapState->shouldBackToMenu = true; };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "chooseMapBackToMenuButton", backToMenuButtonPosition, backToMenuButtonSize, buttonColor, "Back",
        graphics::Color::Black, 37, fontPath, utils::Vector2f{2.0f, 0.0f}, backToMenuClickAction,
        backToMenuButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToMenuButtonConfig));

    const auto rightButtonOnMouseOver = [=]
    {
        chooseMapState->uiManager->setColor(components::ui::UIComponentType::Button, "chooseMapRightButton",
                                            buttonHoverColor);
    };
    const auto rightButtonOnMouseOut = [=]
    {
        chooseMapState->uiManager->setColor(components::ui::UIComponentType::Button, "chooseMapRightButton",
                                            buttonColor);
    };
    auto rightButtonMouseOverActions =
        components::ui::MouseOverActions{rightButtonOnMouseOver, rightButtonOnMouseOut};
    auto rightButtonClickAction = [=] { chooseMapState->showNextMaps(); };
    auto rightButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "chooseMapRightButton", rightArrowButtonPosition, arrowButtonSize, buttonColor, "->",
        graphics::Color::Black, arrowButtonFontSize, fontPath, utils::Vector2f{3.0f, 0.2f},
        rightButtonClickAction, rightButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(rightButtonConfig));

    const auto leftButtonOnMouseOver = [=]
    {
        chooseMapState->uiManager->setColor(components::ui::UIComponentType::Button, "chooseMapLeftButton",
                                            buttonHoverColor);
    };
    const auto leftButtonOnMouseOut = [=]
    {
        chooseMapState->uiManager->setColor(components::ui::UIComponentType::Button, "chooseMapLeftButton",
                                            buttonColor);
    };
    auto leftButtonMouseOverActions =
        components::ui::MouseOverActions{leftButtonOnMouseOver, leftButtonOnMouseOut};
    auto leftButtonClickAction = [=] { chooseMapState->showPreviousMaps(); };
    auto leftButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "chooseMapLeftButton", leftArrowButtonPosition, arrowButtonSize, buttonColor, "<-",
        graphics::Color::Black, arrowButtonFontSize, fontPath, utils::Vector2f{3.0f, 0.2f},
        leftButtonClickAction, leftButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(leftButtonConfig));


    for (auto mapIndex = 0; mapIndex < chooseMapState->mapButtonsUniqueNames.size(); mapIndex++)
    {
        const auto buttonUniqueName = chooseMapState->mapButtonsUniqueNames[mapIndex];
        auto mapNameToDisplay = chooseMapState->mapNames[mapIndex];
        const auto buttonPosition =
            firstMapButtonPosition + utils::Vector2f{0.f, static_cast<float>(mapIndex) * 6.f};
        const auto mapButtonOnMouseOver = [=]
        {
            chooseMapState->uiManager->setColor(components::ui::UIComponentType::Button, buttonUniqueName,
                                                buttonHoverColor);
        };
        const auto mapButtonOnMouseOut = [=]
        {
            chooseMapState->uiManager->setColor(components::ui::UIComponentType::Button, buttonUniqueName,
                                                buttonColor);
        };
        auto mapButtonMouseOverActions =
            components::ui::MouseOverActions{mapButtonOnMouseOver, mapButtonOnMouseOut};
        auto mapButtonClickAction = [=] {};
        auto mapButtonConfig = std::make_unique<components::ui::ButtonConfig>(
            buttonUniqueName, buttonPosition, mapButtonSize, buttonColor, mapNameToDisplay,
            graphics::Color::Black, arrowButtonFontSize, fontPath, utils::Vector2f{2.0f, 0.7f},
            mapButtonClickAction, mapButtonMouseOverActions);
        buttonsConfig.emplace_back(std::move(mapButtonConfig));
    }

    return buttonsConfig;
}

std::vector<std::unique_ptr<components::ui::CheckBoxConfig>>
ChooseMapStateUIConfigBuilder::createCheckBoxConfigs(ChooseMapState*)
{
    return {};
}

std::vector<std::unique_ptr<components::ui::LabelConfig>>
ChooseMapStateUIConfigBuilder::createLabelConfigs(ChooseMapState*)
{
    std::vector<std::unique_ptr<components::ui::LabelConfig>> labelsConfig;

    auto titleLabelConfig = std::make_unique<components::ui::LabelConfig>(
        "chooseMapTitleLabel", titlePosition, graphics::Color::Black, "Choose Map", 37, fontPath);
    labelsConfig.emplace_back(std::move(titleLabelConfig));

    return labelsConfig;
}

std::vector<std::unique_ptr<components::ui::TextFieldConfig>>
ChooseMapStateUIConfigBuilder::createTextFieldConfigs(ChooseMapState*)
{
    return {};
}

}