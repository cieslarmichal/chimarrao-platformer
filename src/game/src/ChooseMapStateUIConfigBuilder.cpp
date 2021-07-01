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
const auto iconSize = utils::Vector2f{4, 4};
}

const std::vector<std::string> ChooseMapStateUIConfigBuilder::mapButtonsUniqueNames{
    "chooseMap1MapButton", "chooseMap2MapButton", "chooseMap3MapButton", "chooseMap4MapButton",
    "chooseMap5MapButton"};

const std::vector<std::string> ChooseMapStateUIConfigBuilder::iconNames{
    "chooseMapIcon1Image", "chooseMapIcon2Image", "chooseMapIcon3Image",
    "chooseMapIcon4Image", "chooseMapIcon5Image", "chooseMapIcon6Image"};

std::unique_ptr<components::ui::UIConfig>
ChooseMapStateUIConfigBuilder::createChooseMapUIConfig(ChooseMapState* chooseMapState)
{
    return std::make_unique<components::ui::UIConfig>(
        createBackgroundConfig(chooseMapState), createButtonConfigs(chooseMapState),
        createCheckBoxConfigs(chooseMapState), createLabelConfigs(chooseMapState),
        createTextFieldConfigs(chooseMapState), createImageConfigs(chooseMapState));
}

std::vector<std::string> ChooseMapStateUIConfigBuilder::getNonNavigationButtonNames()
{
    auto buttonNames = mapButtonsUniqueNames;
    buttonNames.emplace_back("chooseMapBackToMenuButton");
    return buttonNames;
}

std::vector<std::string> ChooseMapStateUIConfigBuilder::getIconNames()
{
    return iconNames;
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
        chooseMapState->buttonsNavigator->setFocusOnButton("chooseMapBackToMenuButton");
        chooseMapState->uiManager->setColor("chooseMapBackToMenuButton", buttonHoverColor);
    };
    const auto backToMenuButtonOnMouseOut = [=]
    {
        chooseMapState->buttonsNavigator->loseFocus();
        chooseMapState->uiManager->setColor("chooseMapBackToMenuButton", buttonColor);
    };
    auto backToMenuButtonMouseOverActions =
        components::ui::MouseOverActions{backToMenuButtonOnMouseOver, backToMenuButtonOnMouseOut};
    auto backToMenuClickAction = [=] { chooseMapState->backToMenu(); };
    auto backToMenuButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "chooseMapBackToMenuButton", backToMenuButtonPosition, backToMenuButtonSize, buttonColor, "Back",
        graphics::Color::Black, 37, fontPath, utils::Vector2f{2.0f, 0.0f}, backToMenuClickAction,
        backToMenuButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(backToMenuButtonConfig));

    const auto rightButtonOnMouseOver = [=]
    { chooseMapState->uiManager->setColor("chooseMapRightButton", buttonHoverColor); };
    const auto rightButtonOnMouseOut = [=]
    { chooseMapState->uiManager->setColor("chooseMapRightButton", buttonColor); };
    auto rightButtonMouseOverActions =
        components::ui::MouseOverActions{rightButtonOnMouseOver, rightButtonOnMouseOut};
    auto rightButtonClickAction = [=] { chooseMapState->buttonsNavigator->changePageToRight(); };
    auto rightButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "chooseMapRightButton", rightArrowButtonPosition, arrowButtonSize, buttonColor, "->",
        graphics::Color::Black, arrowButtonFontSize, fontPath, utils::Vector2f{3.0f, 0.2f},
        rightButtonClickAction, rightButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(rightButtonConfig));

    const auto leftButtonOnMouseOver = [=]
    { chooseMapState->uiManager->setColor("chooseMapLeftButton", buttonHoverColor); };
    const auto leftButtonOnMouseOut = [=]
    { chooseMapState->uiManager->setColor("chooseMapLeftButton", buttonColor); };
    auto leftButtonMouseOverActions =
        components::ui::MouseOverActions{leftButtonOnMouseOver, leftButtonOnMouseOut};
    auto leftButtonClickAction = [=] { chooseMapState->buttonsNavigator->changePageToLeft(); };
    auto leftButtonConfig = std::make_unique<components::ui::ButtonConfig>(
        "chooseMapLeftButton", leftArrowButtonPosition, arrowButtonSize, buttonColor, "<-",
        graphics::Color::Black, arrowButtonFontSize, fontPath, utils::Vector2f{3.0f, 0.2f},
        leftButtonClickAction, leftButtonMouseOverActions);
    buttonsConfig.emplace_back(std::move(leftButtonConfig));

    for (std::size_t mapIndex = 0; mapIndex < mapButtonsUniqueNames.size(); mapIndex++)
    {
        const auto& buttonUniqueName = mapButtonsUniqueNames[mapIndex];
        auto& mapNameToDisplay = chooseMapState->mapNames[mapIndex];
        auto& mapFilePath = chooseMapState->mapFilePaths[mapIndex];
        const auto buttonPosition =
            firstMapButtonPosition + utils::Vector2f{0.f, static_cast<float>(mapIndex) * 6.f};
        const auto mapButtonOnMouseOver = [=]
        {
            chooseMapState->buttonsNavigator->setFocusOnButton(buttonUniqueName);
            chooseMapState->uiManager->setColor(buttonUniqueName, buttonHoverColor);
        };
        const auto mapButtonOnMouseOut = [=]
        {
            chooseMapState->buttonsNavigator->loseFocus();
            chooseMapState->uiManager->setColor(buttonUniqueName, buttonColor);
        };
        auto mapButtonMouseOverActions =
            components::ui::MouseOverActions{mapButtonOnMouseOver, mapButtonOnMouseOut};
        auto mapButtonClickAction = [=, &mapFilePath]
        {
            chooseMapState->tileMap->loadFromFile(mapFilePath);
            chooseMapState->states.deactivateCurrentState();
            chooseMapState->states.addNextState(StateType::Game);
        };
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

std::vector<std::unique_ptr<components::ui::ImageConfig>>
ChooseMapStateUIConfigBuilder::createImageConfigs(ChooseMapState*)
{
    std::vector<std::unique_ptr<components::ui::ImageConfig>> imagesConfig;

    for (std::size_t iconIndex = 0; iconIndex < iconNames.size(); iconIndex++)
    {
        auto iconPosition =
            utils::Vector2f{firstMapButtonPosition.x + mapButtonSize.x,
                            firstMapButtonPosition.y + static_cast<float>(iconIndex) * 6.f - 0.5f};
        if (iconIndex == iconNames.size() - 1)
        {
            iconPosition.x -= 1.f;
            iconPosition.y += 3.5f;
        }

        auto imageConfig = std::make_unique<components::ui::ImageConfig>(
            iconNames[iconIndex], iconPosition, iconSize, graphics::VisibilityLayer::First, iconPath);
        imagesConfig.push_back(std::move(imageConfig));
    }

    return imagesConfig;
}

}